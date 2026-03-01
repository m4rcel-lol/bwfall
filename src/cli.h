#ifndef CLI_H
#define CLI_H

#include <stdbool.h>
#include <stddef.h>

typedef enum {
    AUDIO_MODE_PCM,
    AUDIO_MODE_FREQ,
    AUDIO_MODE_FM,
    AUDIO_MODE_NOISE
} audio_mode_t;

typedef enum {
    VISUAL_MODE_WATERFALL,
    VISUAL_MODE_HEX,
    VISUAL_MODE_BINARY,
    VISUAL_MODE_MATRIX,
    VISUAL_MODE_SPECTRUM,
    VISUAL_MODE_WAVEFORM
} visual_mode_t;

typedef enum {
    COLORMAP_RAINBOW,
    COLORMAP_HEAT,
    COLORMAP_GRAYSCALE,
    COLORMAP_BINARY
} colormap_t;

typedef struct {
    const char *input_file;
    bool use_stdin;
    bool no_audio;
    bool no_visual;
    float speed;
    bool loop;
    bool start_paused;
    size_t start_offset;
    size_t length;
    audio_mode_t audio_mode;
    int sample_rate;
    int channels;
    int volume;
    int freq_min;
    int freq_max;
    visual_mode_t visual_mode;
    colormap_t colormap;
    int fps;
    bool verbose;
    bool show_stats;
    size_t buffer_size;
} config_t;

void config_init(config_t *cfg);
bool parse_args(int argc, char **argv, config_t *cfg);
void print_help(const char *progname);
void print_version(void);

#endif
