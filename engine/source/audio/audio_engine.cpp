#include <engine/audio/audio_engine.hpp>

#include <stdio.h>

namespace paper
{

    AudioEngine::AudioEngine()
    {
        if (ma_engine_init(NULL, &m_engine) != MA_SUCCESS)
            printf("Couldn't initialize audio engine.");
    }

    AudioEngine::~AudioEngine()
    {
        ma_engine_uninit(&m_engine);
    }

    void AudioEngine::play_sound(const char *path)
    {
        ma_engine_play_sound(&m_engine, path, NULL);
    }

} // namespace paper
