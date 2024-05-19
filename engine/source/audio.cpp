#include <engine/audio.hpp>

#include <stdio.h>

namespace paper
{

    Audio::Audio()
    {
        if (ma_engine_init(NULL, &m_engine) != MA_SUCCESS)
            printf("Couldn't initialize audio engine.");
    }

    Audio::~Audio()
    {
        ma_engine_uninit(&m_engine);
    }

    void Audio::play_sound(const char *path)
    {
        ma_engine_play_sound(&m_engine, path, NULL);
    }

} // namespace paper
