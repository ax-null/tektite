#include <core/audio/audio.hpp>

#include "../third_party/miniaudio.h"
#include <stdio.h>

namespace Tektite
{

    static ma_engine engine;

    Audio::Audio()
    {
        if (ma_engine_init(NULL, &engine) != MA_SUCCESS)
            printf("Couldn't initialize audio engine.");
    }

    Audio::~Audio()
    {
        ma_engine_uninit(&engine);
    }

    void Audio::playSound(const char *path)
    {
        ma_engine_play_sound(&engine, path, NULL);
    }

} // namespace Tektite
