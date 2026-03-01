#ifndef AUDIO_H
#define AUDIO_H

#include "ringbuf.h"
#include "cli.h"
#include <pthread.h>

// Forward declaration
typedef struct visual_s visual_t;

typedef struct {
    ringbuf_t *ringbuf;
    config_t *config;
    pthread_t thread;
    volatile bool running;
    volatile bool paused;
    volatile int volume;
    visual_t *visual;
} audio_t;

audio_t* audio_create(ringbuf_t *rb, config_t *cfg);
void audio_start(audio_t *a);
void audio_stop(audio_t *a);
void audio_pause(audio_t *a);
void audio_resume(audio_t *a);
void audio_set_volume(audio_t *a, int vol);
void audio_destroy(audio_t *a);

#endif
