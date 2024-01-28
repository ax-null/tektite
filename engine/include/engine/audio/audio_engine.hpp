#pragma once

namespace tektite
{

    class AudioEngine
    {
      public:
        AudioEngine();
        ~AudioEngine();

        void play_sound(const char *path);
    };

} // namespace tektite
