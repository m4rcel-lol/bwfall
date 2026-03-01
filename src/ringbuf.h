#ifndef RINGBUF_H
#define RINGBUF_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct {
    uint8_t *buffer;
    size_t size;
    volatile size_t write_pos;
    volatile size_t read_pos_audio;
    volatile size_t read_pos_visual;
    volatile bool eof;
} ringbuf_t;

ringbuf_t* ringbuf_create(size_t size);
void ringbuf_destroy(ringbuf_t *rb);
size_t ringbuf_write(ringbuf_t *rb, const uint8_t *data, size_t len);
size_t ringbuf_read_audio(ringbuf_t *rb, uint8_t *data, size_t len);
size_t ringbuf_read_visual(ringbuf_t *rb, uint8_t *data, size_t len);
size_t ringbuf_available_write(ringbuf_t *rb);
size_t ringbuf_available_read_audio(ringbuf_t *rb);
void ringbuf_set_eof(ringbuf_t *rb);
bool ringbuf_is_eof(ringbuf_t *rb);

#endif
