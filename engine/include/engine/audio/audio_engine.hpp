#pragma once

#include <miniaudio.h>

namespace paper
{

    class AudioEngine
    {
      public:
        AudioEngine();
        ~AudioEngine();

        void play_sound(const char *path);

      private:
        ma_engine m_engine;
    };

} // namespace paper
