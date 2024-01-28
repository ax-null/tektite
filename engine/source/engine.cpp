#include <engine/engine.hpp>
#include <engine/world/tilemap.hpp>

#include <string>

namespace tektite
{

    Engine::Engine()
    {
        m_running = true;

        m_audio_engine = std::make_unique<AudioEngine>();
        m_graphics = std::make_unique<Graphics>(640, 480);
        m_input_system = std::make_unique<InputSystem>();
        m_script_engine = std::make_unique<ScriptEngine>();
        m_world = std::make_unique<World>(m_script_engine.get());

        m_font = std::make_unique<Font>("assets/fonts/normal_font.fnt");

        m_script_engine->initialize_module(m_audio_engine.get());
        m_script_engine->initialize_module(m_graphics.get());
        m_script_engine->initialize_module(m_input_system.get());
        m_script_engine->initialize_module(m_world.get());

        m_script_engine->define_objects();

        m_world->load_map("assets/maps/test_map.json");
    }

    void Engine::run()
    {
        m_input_system->update();
        m_world->update();

        Camera *camera = m_graphics->get_camera();
        float map_width = m_world->get_tilemap()->get_size_x() * m_world->get_tilemap()->get_tile_width();
        float map_height = m_world->get_tilemap()->get_size_y() * m_world->get_tilemap()->get_tile_height();
        camera->keep_in_bounds(0, 0, map_width - m_graphics->get_virtual_width(), map_height - m_graphics->get_virtual_height());
        camera->update();

        if (m_input_system->is_pressed(keyboard::Escape))
            m_running = false;

        m_graphics->clear(0.25, 0.25, 0.25, 1.0);
        m_world->render(m_graphics.get());

        std::string text;
        text.append("Draw Calls: ");
        text.append(std::to_string(m_graphics->draw_calls));
        text.append("\nCamera: ");
        text.append(std::to_string(camera->get_x()));
        text.append(", ");
        text.append(std::to_string(camera->get_y()));
        text.append("\nMouse:");
        text.append(std::to_string(m_input_system->get_x()));
        text.append(", ");
        text.append(std::to_string(m_input_system->get_y()));
        text.append("\n");

        m_graphics->set_color(1.0f, 1.0f, 1.0f, 1.0f);
        m_graphics->draw_string(m_font.get(), text.c_str(), camera->get_x() + 10, camera->get_y() + (m_graphics->get_virtual_height() - m_font->get_line_height()) - 10);
        m_graphics->draw_calls = 0;

        m_graphics->submit();
    }

} // namespace tektite
