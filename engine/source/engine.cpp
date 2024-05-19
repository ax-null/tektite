#include <engine/engine.hpp>

#include <glm/common.hpp>
#include <string>

namespace paper
{

    Engine::Engine()
    {
        m_running = true;

        m_audio = std::make_unique<Audio>();
        m_graphics = std::make_unique<Graphics>(640, 480);
        m_input = std::make_unique<Input>();
        m_script = std::make_unique<Script>();
        m_world = std::make_unique<World>(m_script.get());

        m_font = std::make_unique<Font>("assets/fonts/normal_font.fnt");

        m_script->initialize_module(m_audio.get());
        m_script->initialize_module(m_graphics.get());
        m_script->initialize_module(m_input.get());
        m_script->initialize_module(m_world.get());

        m_script->define_objects();

        m_world->load_map("assets/maps/test_map.json");

        m_inactive_sleep_time = time::from_seconds(1.0f / 30);
        m_target_elapsed_time = time::from_seconds(1.0f / 60);
        m_max_elapsed_time = time::from_milliseconds(500);

        m_fixed_timestep = false;
        m_vertical_sync = true;
    }

    void Engine::update()
    {
        m_input->update();
        m_world->update();

        Camera *camera = m_graphics->get_camera();
        float map_width = m_world->get_tilemap()->get_size_x() * m_world->get_tilemap()->get_tile_width();
        float map_height = m_world->get_tilemap()->get_size_y() * m_world->get_tilemap()->get_tile_height();
        camera->keep_in_bounds(0, 0, map_width - m_graphics->get_virtual_width(), map_height - m_graphics->get_virtual_height());
        camera->update();

        if (m_input->is_pressed(keyboard::Escape))
            m_running = false;
    }

    void Engine::render()
    {
        m_graphics->clear(0.25, 0.25, 0.25, 1.0);
        m_world->render(m_graphics.get());

        Camera *camera = m_graphics->get_camera();

        std::string text;
        text.append("FPS: ");
        text.append(std::to_string(m_frames_per_second));
        text.append("\nTotal Time: ");
        text.append(std::to_string(time::to_seconds(m_total_time)));
        text.append("\nDelta Time: ");
        text.append(std::to_string(m_delta_time));
        text.append("\nDraw Calls: ");
        text.append(std::to_string(m_graphics->draw_calls));
        text.append("\nCamera: ");
        text.append(std::to_string(camera->get_x()));
        text.append(", ");
        text.append(std::to_string(camera->get_y()));
        text.append("\nMouse:");
        text.append(std::to_string(m_input->get_x()));
        text.append(", ");
        text.append(std::to_string(m_input->get_y()));
        text.append("\n");

        m_graphics->set_color(1.0f, 1.0f, 1.0f, 1.0f);
        m_graphics->draw_string(m_font.get(), text.c_str(), camera->get_x() + 10, camera->get_y() + (m_graphics->get_virtual_height() - m_font->get_line_height()) - 10);
        m_graphics->draw_calls = 0;

        m_graphics->submit();
    }

    void Engine::run(time::nano current_time)
    {
        m_accumulated_time += current_time - m_last_time;
        m_last_time = current_time;

        if (m_fixed_timestep && m_accumulated_time < m_target_elapsed_time) {
            m_sleep_time = m_target_elapsed_time - m_accumulated_time;
            return;
        } else
            m_sleep_time = 0;

        if (m_accumulated_time > m_max_elapsed_time)
            m_accumulated_time = m_max_elapsed_time;

        m_last_elapsed_time += time::to_seconds(m_elapsed_time);
        m_frames_count++;

        if (m_last_elapsed_time >= 1.0f) {
            m_frames_per_second = m_frames_count / m_last_elapsed_time;
            m_frames_count = 0;
            m_last_elapsed_time = 0;
        }

        if (m_fixed_timestep) {
            m_elapsed_time = m_target_elapsed_time;
            int32_t step_count = 0;

            while (m_accumulated_time >= m_target_elapsed_time && m_running) {
                m_total_time += m_target_elapsed_time;
                m_accumulated_time -= m_target_elapsed_time;
                m_delta_time = time::to_seconds(m_elapsed_time);

                ++step_count;

                update();
            }

            m_update_lag = glm::max(0, step_count - 1);

            if (m_running_slowly) {
                if (m_update_lag == 0)
                    m_running_slowly = false;
            } else if (m_update_lag >= 5)
                m_running_slowly = true;

            if (step_count == 0 && m_update_lag > 0)
                --m_update_lag;

            m_elapsed_time = m_target_elapsed_time * step_count;
        } else {
            m_elapsed_time = m_accumulated_time;
            m_total_time += m_accumulated_time;
            m_accumulated_time = 0;
            m_delta_time = time::to_seconds(m_elapsed_time);

            update();
        }

        render();
    }

} // namespace paper
