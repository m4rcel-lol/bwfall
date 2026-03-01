#include "audio.h"
#include "visual.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <alsa/asoundlib.h>
#include <math.h>

static void* audio_thread(void *arg) {
    audio_t *a = (audio_t*)arg;
    snd_pcm_t *handle = NULL;
    
    int err = snd_pcm_open(&handle, "default", SND_PCM_STREAM_PLAYBACK, 0);
    if (err < 0) {
        fprintf(stderr, "Audio: Cannot open ALSA device: %s\n", snd_strerror(err));
        return NULL;
    }
    
    snd_pcm_hw_params_t *params;
    snd_pcm_hw_params_alloca(&params);
    snd_pcm_hw_params_any(handle, params);
    snd_pcm_hw_params_set_access(handle, params, SND_PCM_ACCESS_RW_INTERLEAVED);
    snd_pcm_hw_params_set_format(handle, params, SND_PCM_FORMAT_U8);
    snd_pcm_hw_params_set_channels(handle, params, a->config->channels);
    
    unsigned int rate = a->config->sample_rate;
    snd_pcm_hw_params_set_rate_near(handle, params, &rate, 0);
    snd_pcm_hw_params(handle, params);
    
    uint8_t buffer[4096];
    
    while (a->running) {
        if (a->paused) {
            usleep(10000);
            continue;
        }
        
        size_t nread = ringbuf_read_audio(a->ringbuf, buffer, sizeof(buffer));
        if (nread == 0) {
            if (ringbuf_is_eof(a->ringbuf)) break;
            usleep(1000);
            continue;
        }
        
        // Apply volume
        if (a->volume != 100) {
            for (size_t i = 0; i < nread; i++) {
                int val = ((int)buffer[i] - 128) * a->volume / 100 + 128;
                if (val < 0) val = 0;
                if (val > 255) val = 255;
                buffer[i] = (uint8_t)val;
            }
        }
        
        snd_pcm_sframes_t frames = snd_pcm_writei(handle, buffer, nread / a->config->channels);
        if (frames < 0) {
            frames = snd_pcm_recover(handle, frames, 0);
        }
        
        // Speed control - slow down playback
        if (a->config->speed < 1.0f) {
            useconds_t delay = (useconds_t)(1000000.0 * nread / rate * (1.0 / a->config->speed - 1.0));
            usleep(delay);
        }
    }
    
    snd_pcm_drain(handle);
    snd_pcm_close(handle);
    
    // Signal visual that audio is done
    if (a->visual) {
        a->visual->audio_finished = true;
    }
    
    return NULL;
}

audio_t* audio_create(ringbuf_t *rb, config_t *cfg) {
    audio_t *a = malloc(sizeof(audio_t));
    if (!a) return NULL;
    
    a->ringbuf = rb;
    a->config = cfg;
    a->running = false;
    a->paused = cfg->start_paused;
    a->volume = cfg->volume;
    a->visual = NULL;
    
    return a;
}

void audio_start(audio_t *a) {
    a->running = true;
    pthread_create(&a->thread, NULL, audio_thread, a);
}

void audio_stop(audio_t *a) {
    a->running = false;
    pthread_join(a->thread, NULL);
}

void audio_pause(audio_t *a) {
    a->paused = true;
}

void audio_resume(audio_t *a) {
    a->paused = false;
}

void audio_set_volume(audio_t *a, int vol) {
    if (vol < 0) vol = 0;
    if (vol > 100) vol = 100;
    a->volume = vol;
}

void audio_destroy(audio_t *a) {
    if (a) {
        free(a);
    }
}
