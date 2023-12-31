#pragma once

#include <memory>

#include "../audio/audio.hpp"
#include "../graphics/graphics.hpp"
#include "../input/input.hpp"
#include "../script/script.hpp"
#include "../world/world.hpp"

namespace Tektite
{
    class Engine
    {
      public:
        Engine();
        ~Engine() = default;

        void run();

        Graphics *getGraphics() const { return m_graphics.get(); }
        Input *getInput() const { return m_input.get(); }
        bool isRunning() const { return m_running; }

      private:
        std::unique_ptr<Audio> m_audio;
        std::unique_ptr<Graphics> m_graphics;
        std::unique_ptr<Input> m_input;
        std::unique_ptr<Script> m_script;
        std::unique_ptr<World> m_world;

        std::unique_ptr<Font> m_font;

        bool m_running;
    };

} // namespace Tektite
