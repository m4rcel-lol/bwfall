#ifndef VISUAL_H
#define VISUAL_H

#include "ringbuf.h"
#include "cli.h"
#include <pthread.h>

typedef struct visual_s {
    ringbuf_t *ringbuf;
    config_t *config;
    pthread_t thread;
    volatile bool running;
    volatile bool paused;
    volatile bool audio_finished;
} visual_t;

visual_t* visual_create(ringbuf_t *rb, config_t *cfg);
void visual_start(visual_t *v);
void visual_stop(visual_t *v);
void visual_pause(visual_t *v);
void visual_resume(visual_t *v);
void visual_destroy(visual_t *v);

#endif
