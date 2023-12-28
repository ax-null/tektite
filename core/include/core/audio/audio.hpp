#pragma once

namespace Tektite
{

    class Audio
    {
      public:
        Audio();
        ~Audio();

        void playSound(const char* path);
    };

} // namespace Tektite
