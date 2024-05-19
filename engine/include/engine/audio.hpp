#pragma once

#include <miniaudio.h>

namespace paper
{

    class Audio
    {
      public:
        Audio();
        ~Audio();

        void play_sound(const char *path);

      private:
        ma_engine m_engine;
    };

} // namespace paper
