#include "visual.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <SDL2/SDL.h>

#define WINDOW_WIDTH 512
#define WINDOW_HEIGHT 512
#define PIXEL_SIZE 4
#define GRID_WIDTH (WINDOW_WIDTH / PIXEL_SIZE)
#define GRID_HEIGHT (WINDOW_HEIGHT / PIXEL_SIZE)

static void get_rgb_color(uint8_t byte, colormap_t cmap, uint8_t *r, uint8_t *g, uint8_t *b) {
    switch (cmap) {
        case COLORMAP_RAINBOW:
            if (byte < 43) { *r = 255; *g = 0; *b = 0; }        // red
            else if (byte < 86) { *r = 255; *g = 127; *b = 0; } // orange
            else if (byte < 128) { *r = 255; *g = 255; *b = 0; }// yellow
            else if (byte < 170) { *r = 0; *g = 255; *b = 0; }  // green
            else if (byte < 213) { *r = 0; *g = 0; *b = 255; }  // blue
            else { *r = 255; *g = 0; *b = 255; }                // magenta
            break;
        case COLORMAP_HEAT:
            if (byte < 85) { *r = 0; *g = 0; *b = byte * 3; }
            else if (byte < 170) { *r = (byte - 85) * 3; *g = 0; *b = 255 - (byte - 85) * 3; }
            else { *r = 255; *g = (byte - 170) * 3; *b = 0; }
            break;
        case COLORMAP_GRAYSCALE:
            *r = *g = *b = byte;
            break;
        case COLORMAP_BINARY:
            if (byte > 127) { *r = *g = *b = 255; }
            else { *r = *g = *b = 0; }
            break;
        default:
            *r = *g = *b = byte;
    }
}

static void* visual_thread(void *arg) {
    visual_t *v = (visual_t*)arg;
    
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "SDL init failed: %s\n", SDL_GetError());
        return NULL;
    }
    
    SDL_Window *window = SDL_CreateWindow(
        "Binary Waterfall",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        SDL_WINDOW_SHOWN
    );
    
    if (!window) {
        fprintf(stderr, "Window creation failed: %s\n", SDL_GetError());
        SDL_Quit();
        return NULL;
    }
    
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        fprintf(stderr, "Renderer creation failed: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return NULL;
    }
    
    SDL_Texture *texture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_RGB888,
        SDL_TEXTUREACCESS_STREAMING,
        GRID_WIDTH,
        GRID_HEIGHT
    );
    
    uint32_t *pixels = malloc(GRID_WIDTH * GRID_HEIGHT * sizeof(uint32_t));
    memset(pixels, 0, GRID_WIDTH * GRID_HEIGHT * sizeof(uint32_t));
    
    uint8_t buffer[128];
    int pixel_index = 0;
    
    while (v->running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                v->running = false;
                break;
            }
        }
        
        if (v->paused) {
            usleep(10000);
            continue;
        }
        
        // Keep reading and updating while audio is playing
        if (!v->audio_finished) {
            size_t nread = ringbuf_read_visual(v->ringbuf, buffer, sizeof(buffer));
            
            if (nread > 0) {
                // Update pixels with new data
                for (size_t i = 0; i < nread; i++) {
                    uint8_t r, g, b;
                    get_rgb_color(buffer[i], v->config->colormap, &r, &g, &b);
                    pixels[pixel_index] = (r << 16) | (g << 8) | b;
                    pixel_index = (pixel_index + 1) % (GRID_WIDTH * GRID_HEIGHT);
                }
            }
        } else {
            // Audio finished - close window
            break;
        }
        
        // Always render to keep display updating
        SDL_UpdateTexture(texture, NULL, pixels, GRID_WIDTH * sizeof(uint32_t));
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);
    }
    
    free(pixels);
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    
    return NULL;
}

visual_t* visual_create(ringbuf_t *rb, config_t *cfg) {
    visual_t *v = malloc(sizeof(visual_t));
    if (!v) return NULL;
    
    v->ringbuf = rb;
    v->config = cfg;
    v->running = false;
    v->paused = cfg->start_paused;
    v->audio_finished = false;
    
    return v;
}

void visual_start(visual_t *v) {
    v->running = true;
    pthread_create(&v->thread, NULL, visual_thread, v);
}

void visual_stop(visual_t *v) {
    v->running = false;
    pthread_join(v->thread, NULL);
}

void visual_pause(visual_t *v) {
    v->paused = true;
}

void visual_resume(visual_t *v) {
    v->paused = false;
}

void visual_destroy(visual_t *v) {
    if (v) {
        free(v);
    }
}
