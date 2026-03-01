#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "../src/ringbuf.h"

void test_create_destroy() {
    ringbuf_t *rb = ringbuf_create(1024);
    assert(rb != NULL);
    assert(rb->size == 1024);
    assert(rb->write_pos == 0);
    assert(rb->read_pos_audio == 0);
    assert(rb->read_pos_visual == 0);
    ringbuf_destroy(rb);
    printf("✓ test_create_destroy\n");
}

void test_write_read() {
    ringbuf_t *rb = ringbuf_create(1024);
    uint8_t write_data[] = {1, 2, 3, 4, 5};
    uint8_t read_data[5] = {0};
    
    size_t written = ringbuf_write(rb, write_data, 5);
    assert(written == 5);
    
    size_t read = ringbuf_read_audio(rb, read_data, 5);
    assert(read == 5);
    assert(memcmp(write_data, read_data, 5) == 0);
    
    ringbuf_destroy(rb);
    printf("✓ test_write_read\n");
}

void test_wrap_around() {
    ringbuf_t *rb = ringbuf_create(10);
    uint8_t data[8] = {1, 2, 3, 4, 5, 6, 7, 8};
    uint8_t read_buf[8];
    
    ringbuf_write(rb, data, 8);
    ringbuf_read_audio(rb, read_buf, 8);
    
    // Write again to test wrap
    ringbuf_write(rb, data, 8);
    ringbuf_read_audio(rb, read_buf, 8);
    assert(memcmp(data, read_buf, 8) == 0);
    
    ringbuf_destroy(rb);
    printf("✓ test_wrap_around\n");
}

void test_eof() {
    ringbuf_t *rb = ringbuf_create(1024);
    assert(!ringbuf_is_eof(rb));
    
    ringbuf_set_eof(rb);
    assert(ringbuf_is_eof(rb));
    
    ringbuf_destroy(rb);
    printf("✓ test_eof\n");
}

int main() {
    printf("Running ringbuf tests...\n");
    test_create_destroy();
    test_write_read();
    test_wrap_around();
    test_eof();
    printf("All tests passed!\n");
    return 0;
}
