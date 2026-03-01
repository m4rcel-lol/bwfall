#include "reader.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static void* reader_thread(void *arg) {
    reader_t *r = (reader_t*)arg;
    FILE *fp = NULL;
    
    if (r->config->use_stdin) {
        fp = stdin;
    } else {
        fp = fopen(r->config->input_file, "rb");
        if (!fp) {
            fprintf(stderr, "Error: Cannot open file %s\n", r->config->input_file);
            ringbuf_set_eof(r->ringbuf);
            return NULL;
        }
        
        if (r->config->start_offset > 0) {
            fseek(fp, r->config->start_offset, SEEK_SET);
        }
    }
    
    uint8_t buffer[4096];
    size_t total_read = 0;
    
    while (r->running) {
        size_t to_read = sizeof(buffer);
        if (r->config->length > 0 && total_read + to_read > r->config->length) {
            to_read = r->config->length - total_read;
            if (to_read == 0) break;
        }
        
        size_t nread = fread(buffer, 1, to_read, fp);
        if (nread == 0) {
            if (r->config->loop && !r->config->use_stdin) {
                fseek(fp, r->config->start_offset, SEEK_SET);
                total_read = 0;
                continue;
            }
            break;
        }
        
        size_t written = 0;
        while (written < nread && r->running) {
            size_t w = ringbuf_write(r->ringbuf, buffer + written, nread - written);
            written += w;
            if (w == 0) usleep(1000);
        }
        
        total_read += nread;
    }
    
    if (fp && fp != stdin) {
        fclose(fp);
    }
    
    ringbuf_set_eof(r->ringbuf);
    return NULL;
}

reader_t* reader_create(ringbuf_t *rb, config_t *cfg) {
    reader_t *r = malloc(sizeof(reader_t));
    if (!r) return NULL;
    
    r->ringbuf = rb;
    r->config = cfg;
    r->running = false;
    
    return r;
}

void reader_start(reader_t *r) {
    r->running = true;
    pthread_create(&r->thread, NULL, reader_thread, r);
}

void reader_stop(reader_t *r) {
    r->running = false;
    pthread_join(r->thread, NULL);
}

void reader_destroy(reader_t *r) {
    if (r) {
        free(r);
    }
}
