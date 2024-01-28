#pragma once

#include <memory>

#include "audio/audio_engine.hpp"
#include "graphics/graphics.hpp"
#include "input/input_system.hpp"
#include "script/script_engine.hpp"
#include "world/world.hpp"

namespace tektite
{
    class Engine
    {
      public:
        Engine();
        ~Engine() = default;

        void run();

        Graphics *get_graphics() const { return m_graphics.get(); }
        InputSystem *get_input_system() const { return m_input_system.get(); }
        bool is_running() const { return m_running; }

      private:
        std::unique_ptr<AudioEngine> m_audio_engine;
        std::unique_ptr<Graphics> m_graphics;
        std::unique_ptr<InputSystem> m_input_system;
        std::unique_ptr<ScriptEngine> m_script_engine;
        std::unique_ptr<World> m_world;

        std::unique_ptr<Font> m_font;

        bool m_running;
    };

} // namespace tektite
