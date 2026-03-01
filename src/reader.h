#ifndef READER_H
#define READER_H

#include "ringbuf.h"
#include "cli.h"
#include <pthread.h>

typedef struct {
    ringbuf_t *ringbuf;
    config_t *config;
    pthread_t thread;
    volatile bool running;
} reader_t;

reader_t* reader_create(ringbuf_t *rb, config_t *cfg);
void reader_start(reader_t *r);
void reader_stop(reader_t *r);
void reader_destroy(reader_t *r);

#endif
