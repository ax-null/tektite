#include <engine/audio/audio_engine.hpp>

#include "../third_party/miniaudio.h"
#include <stdio.h>

namespace tektite
{

    static ma_engine engine;

    AudioEngine::AudioEngine()
    {
        if (ma_engine_init(NULL, &engine) != MA_SUCCESS)
            printf("Couldn't initialize audio engine.");
    }

    AudioEngine::~AudioEngine()
    {
        ma_engine_uninit(&engine);
    }

    void AudioEngine::play_sound(const char *path)
    {
        ma_engine_play_sound(&engine, path, NULL);
    }

} // namespace tektite
