#include "cli.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>

#define VERSION "1.0.0"

void config_init(config_t *cfg) {
    cfg->input_file = NULL;
    cfg->use_stdin = false;
    cfg->no_audio = false;
    cfg->no_visual = false;
    cfg->speed = 1.0f;
    cfg->loop = false;
    cfg->start_paused = false;
    cfg->start_offset = 0;
    cfg->length = 0;
    cfg->audio_mode = AUDIO_MODE_PCM;
    cfg->sample_rate = 44100;
    cfg->channels = 1;
    cfg->volume = 80;
    cfg->freq_min = 20;
    cfg->freq_max = 20000;
    cfg->visual_mode = VISUAL_MODE_WATERFALL;
    cfg->colormap = COLORMAP_RAINBOW;
    cfg->fps = 60;
    cfg->verbose = false;
    cfg->show_stats = false;
    cfg->buffer_size = 1048576; // 1MB
}

void print_version(void) {
    printf("bwfall %s\n", VERSION);
}

void print_help(const char *progname) {
    printf("Binary Waterfall CLI - Raw Data Media Player\n\n");
    printf("USAGE:\n");
    printf("  %s [OPTIONS] <file>\n", progname);
    printf("  cat <file> | %s [OPTIONS]\n\n", progname);
    printf("OPTIONS:\n");
    printf("  -i, --input <file>         Input file (or use stdin)\n");
    printf("  --no-audio                 Disable audio output\n");
    printf("  --no-visual                Disable visual output\n");
    printf("  -s, --speed <float>        Playback speed (default: 1.0)\n");
    printf("  -l, --loop                 Loop playback\n");
    printf("  -p, --pause                Start paused\n");
    printf("  --start <bytes>            Start offset\n");
    printf("  --length <bytes>           Bytes to play\n");
    printf("  --audio-mode <mode>        pcm|freq|fm|noise (default: pcm)\n");
    printf("  --sample-rate <hz>         Sample rate (default: 44100)\n");
    printf("  --channels <n>             Channels 1-2 (default: 1)\n");
    printf("  --volume <0-100>           Volume (default: 80)\n");
    printf("  --freq-min <hz>            Min frequency (default: 20)\n");
    printf("  --freq-max <hz>            Max frequency (default: 20000)\n");
    printf("  --visual-mode <mode>       waterfall|hex|binary|matrix|spectrum|waveform\n");
    printf("  --colormap <map>           rainbow|heat|grayscale|binary\n");
    printf("  --fps <n>                  Target FPS (default: 30)\n");
    printf("  -v, --verbose              Verbose output\n");
    printf("  --stats                    Show statistics\n");
    printf("  -h, --help                 Show this help\n");
    printf("  --version                  Show version\n");
}

bool parse_args(int argc, char **argv, config_t *cfg) {
    static struct option long_options[] = {
        {"input", required_argument, 0, 'i'},
        {"no-audio", no_argument, 0, 1},
        {"no-visual", no_argument, 0, 2},
        {"speed", required_argument, 0, 's'},
        {"loop", no_argument, 0, 'l'},
        {"pause", no_argument, 0, 'p'},
        {"start", required_argument, 0, 3},
        {"length", required_argument, 0, 4},
        {"audio-mode", required_argument, 0, 5},
        {"sample-rate", required_argument, 0, 6},
        {"channels", required_argument, 0, 7},
        {"volume", required_argument, 0, 8},
        {"freq-min", required_argument, 0, 9},
        {"freq-max", required_argument, 0, 10},
        {"visual-mode", required_argument, 0, 11},
        {"colormap", required_argument, 0, 12},
        {"fps", required_argument, 0, 13},
        {"verbose", no_argument, 0, 'v'},
        {"stats", no_argument, 0, 14},
        {"help", no_argument, 0, 'h'},
        {"version", no_argument, 0, 15},
        {0, 0, 0, 0}
    };
    
    int opt;
    while ((opt = getopt_long(argc, argv, "i:s:lpvh", long_options, NULL)) != -1) {
        switch (opt) {
            case 'i': cfg->input_file = optarg; break;
            case 1: cfg->no_audio = true; break;
            case 2: cfg->no_visual = true; break;
            case 's': cfg->speed = atof(optarg); break;
            case 'l': cfg->loop = true; break;
            case 'p': cfg->start_paused = true; break;
            case 3: cfg->start_offset = atoll(optarg); break;
            case 4: cfg->length = atoll(optarg); break;
            case 5:
                if (strcmp(optarg, "pcm") == 0) cfg->audio_mode = AUDIO_MODE_PCM;
                else if (strcmp(optarg, "freq") == 0) cfg->audio_mode = AUDIO_MODE_FREQ;
                else if (strcmp(optarg, "fm") == 0) cfg->audio_mode = AUDIO_MODE_FM;
                else if (strcmp(optarg, "noise") == 0) cfg->audio_mode = AUDIO_MODE_NOISE;
                break;
            case 6: cfg->sample_rate = atoi(optarg); break;
            case 7: cfg->channels = atoi(optarg); break;
            case 8: cfg->volume = atoi(optarg); break;
            case 9: cfg->freq_min = atoi(optarg); break;
            case 10: cfg->freq_max = atoi(optarg); break;
            case 11:
                if (strcmp(optarg, "waterfall") == 0) cfg->visual_mode = VISUAL_MODE_WATERFALL;
                else if (strcmp(optarg, "hex") == 0) cfg->visual_mode = VISUAL_MODE_HEX;
                else if (strcmp(optarg, "binary") == 0) cfg->visual_mode = VISUAL_MODE_BINARY;
                else if (strcmp(optarg, "matrix") == 0) cfg->visual_mode = VISUAL_MODE_MATRIX;
                else if (strcmp(optarg, "spectrum") == 0) cfg->visual_mode = VISUAL_MODE_SPECTRUM;
                else if (strcmp(optarg, "waveform") == 0) cfg->visual_mode = VISUAL_MODE_WAVEFORM;
                break;
            case 12:
                if (strcmp(optarg, "rainbow") == 0) cfg->colormap = COLORMAP_RAINBOW;
                else if (strcmp(optarg, "heat") == 0) cfg->colormap = COLORMAP_HEAT;
                else if (strcmp(optarg, "grayscale") == 0) cfg->colormap = COLORMAP_GRAYSCALE;
                else if (strcmp(optarg, "binary") == 0) cfg->colormap = COLORMAP_BINARY;
                break;
            case 13: cfg->fps = atoi(optarg); break;
            case 'v': cfg->verbose = true; break;
            case 14: cfg->show_stats = true; break;
            case 'h': print_help(argv[0]); exit(0);
            case 15: print_version(); exit(0);
            default: return false;
        }
    }
    
    if (optind < argc) {
        cfg->input_file = argv[optind];
    }
    
    return true;
}
