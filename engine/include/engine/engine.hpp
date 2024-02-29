#pragma once

#include <memory>

#include "time.hpp"

#include "audio/audio_engine.hpp"
#include "graphics/graphics.hpp"
#include "input/input_system.hpp"
#include "script/script_engine.hpp"
#include "world/world.hpp"

namespace paper
{

    class Engine
    {
      public:
        Engine();
        ~Engine() = default;

        void run(time::nano current_time);

        Graphics *get_graphics() const { return m_graphics.get(); }
        InputSystem *get_input_system() const { return m_input_system.get(); }

        bool is_running() const { return m_running; }
        bool is_vsync_enabled() const { return m_vertical_sync; }

        time::nano get_sleep_time() const { return m_sleep_time; }
        time::nano get_elapsed_time() const { return m_elapsed_time; }
        time::nano get_total_time() const { return m_total_time; }

      private:
        void update();
        void render();

        std::unique_ptr<AudioEngine> m_audio_engine;
        std::unique_ptr<Graphics> m_graphics;
        std::unique_ptr<InputSystem> m_input_system;
        std::unique_ptr<ScriptEngine> m_script_engine;
        std::unique_ptr<World> m_world;

        std::unique_ptr<Font> m_font;

        time::nano m_inactive_sleep_time;
        time::nano m_max_elapsed_time;
        time::nano m_target_elapsed_time;

        time::nano m_sleep_time, m_elapsed_time, m_total_time;
        float m_frames_per_second, m_delta_time;

        time::nano m_last_time, m_accumulated_time;
        uint32_t m_update_lag, m_frames_count;
        float m_last_elapsed_time;

        bool m_running;

        bool m_vertical_sync;
        bool m_fixed_timestep;
        bool m_running_slowly;
    };

} // namespace paper
