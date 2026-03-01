#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <signal.h>
#include "cli.h"
#include "ringbuf.h"
#include "reader.h"
#include "audio.h"
#include "visual.h"

static volatile bool g_running = true;

static void signal_handler(int sig) {
    (void)sig;
    g_running = false;
}

int main(int argc, char **argv) {
    config_t config;
    config_init(&config);
    
    if (!parse_args(argc, argv, &config)) {
        print_help(argv[0]);
        return 1;
    }
    
    // Check if stdin or file
    if (!config.input_file) {
        if (isatty(STDIN_FILENO)) {
            fprintf(stderr, "Error: No input file specified and stdin is a tty\n");
            print_help(argv[0]);
            return 1;
        }
        config.use_stdin = true;
    }
    
    if (config.no_audio && config.no_visual) {
        fprintf(stderr, "Error: Cannot disable both audio and visual\n");
        return 1;
    }
    
    // Setup signal handlers
    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);
    
    // Create ring buffer
    ringbuf_t *rb = ringbuf_create(config.buffer_size);
    if (!rb) {
        fprintf(stderr, "Error: Failed to create ring buffer\n");
        return 1;
    }
    
    // Create components
    reader_t *reader = reader_create(rb, &config);
    audio_t *audio = NULL;
    visual_t *visual = NULL;
    
    if (!config.no_audio) {
        audio = audio_create(rb, &config);
        if (!audio) {
            fprintf(stderr, "Warning: Failed to create audio engine\n");
        }
    }
    
    if (!config.no_visual) {
        visual = visual_create(rb, &config);
        if (!visual) {
            fprintf(stderr, "Warning: Failed to create visual engine\n");
        }
    }
    
    // Link audio and visual
    if (audio && visual) {
        audio->visual = visual;
    }
    
    // Start threads
    reader_start(reader);
    
    if (audio) {
        audio_start(audio);
    }
    
    if (visual) {
        visual_start(visual);
        // If visual is running, wait for it to finish (it blocks)
        pthread_join(visual->thread, NULL);
        g_running = false;
    } else {
        // Audio only - wait in main loop
        while (g_running && !ringbuf_is_eof(rb)) {
            usleep(100000);
        }
    }
    
    // Cleanup
    reader_stop(reader);
    
    if (audio) {
        audio_stop(audio);
        audio_destroy(audio);
    }
    
    if (visual && visual->running) {
        visual_stop(visual);
        visual_destroy(visual);
    } else if (visual) {
        visual_destroy(visual);
    }
    
    reader_destroy(reader);
    ringbuf_destroy(rb);
    
    return 0;
}
