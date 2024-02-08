#pragma once

namespace paper
{

    class AudioEngine
    {
      public:
        AudioEngine();
        ~AudioEngine();

        void play_sound(const char *path);
    };

} // namespace paper
