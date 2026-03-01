#include "ringbuf.h"
#include <stdlib.h>
#include <string.h>

ringbuf_t* ringbuf_create(size_t size) {
    ringbuf_t *rb = malloc(sizeof(ringbuf_t));
    if (!rb) return NULL;
    
    rb->buffer = malloc(size);
    if (!rb->buffer) {
        free(rb);
        return NULL;
    }
    
    rb->size = size;
    rb->write_pos = 0;
    rb->read_pos_audio = 0;
    rb->read_pos_visual = 0;
    rb->eof = false;
    
    return rb;
}

void ringbuf_destroy(ringbuf_t *rb) {
    if (rb) {
        free(rb->buffer);
        free(rb);
    }
}

size_t ringbuf_write(ringbuf_t *rb, const uint8_t *data, size_t len) {
    size_t written = 0;
    while (written < len && ringbuf_available_write(rb) > 0) {
        rb->buffer[rb->write_pos] = data[written];
        rb->write_pos = (rb->write_pos + 1) % rb->size;
        written++;
    }
    return written;
}

size_t ringbuf_read_audio(ringbuf_t *rb, uint8_t *data, size_t len) {
    size_t read = 0;
    while (read < len) {
        if (rb->read_pos_audio == rb->write_pos) {
            if (rb->eof) break;
            continue;
        }
        data[read] = rb->buffer[rb->read_pos_audio];
        rb->read_pos_audio = (rb->read_pos_audio + 1) % rb->size;
        read++;
    }
    return read;
}

size_t ringbuf_read_visual(ringbuf_t *rb, uint8_t *data, size_t len) {
    size_t read = 0;
    
    // Copy from current visual position
    while (read < len) {
        size_t available = 0;
        if (rb->write_pos >= rb->read_pos_visual) {
            available = rb->write_pos - rb->read_pos_visual;
        } else {
            available = rb->size - rb->read_pos_visual + rb->write_pos;
        }
        
        if (available == 0) {
            // No data available, loop back to start if there's any data at all
            if (rb->write_pos > 0) {
                rb->read_pos_visual = 0;
                continue;
            }
            break;
        }
        
        data[read] = rb->buffer[rb->read_pos_visual];
        rb->read_pos_visual = (rb->read_pos_visual + 1) % rb->size;
        read++;
    }
    return read;
}

size_t ringbuf_available_write(ringbuf_t *rb) {
    size_t wp = rb->write_pos;
    size_t rp_audio = rb->read_pos_audio;
    size_t rp_visual = rb->read_pos_visual;
    size_t min_rp = (rp_audio < rp_visual) ? rp_audio : rp_visual;
    
    if (wp >= min_rp) {
        return rb->size - (wp - min_rp) - 1;
    } else {
        return min_rp - wp - 1;
    }
}

size_t ringbuf_available_read_audio(ringbuf_t *rb) {
    size_t wp = rb->write_pos;
    size_t rp = rb->read_pos_audio;
    
    if (wp >= rp) {
        return wp - rp;
    } else {
        return rb->size - rp + wp;
    }
}

void ringbuf_set_eof(ringbuf_t *rb) {
    rb->eof = true;
}

bool ringbuf_is_eof(ringbuf_t *rb) {
    return rb->eof && rb->read_pos_audio == rb->write_pos;
}
