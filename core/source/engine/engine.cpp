#include "core/world/tilemap.hpp"
#include <core/engine/engine.hpp>
#include <string>

namespace Tektite
{
    std::unique_ptr<TileMap> tileMap;
    std::string text;

    Engine::Engine()
    {
        m_running = true;

        m_audio = std::make_unique<Audio>();
        m_graphics = std::make_unique<Graphics>(640, 480);
        m_input = std::make_unique<Input>();
        m_script = std::make_unique<Script>();
        m_world = std::make_unique<World>(m_script.get());

        m_font = std::make_unique<Font>("assets/fonts/normal_font.fnt");

        m_script->initializeModule(m_audio.get());
        m_script->initializeModule(m_graphics.get());
        m_script->initializeModule(m_input.get());
        m_script->initializeModule(m_world.get());

        m_script->defineObjects();

        m_world->loadMap("assets/maps/test_map.json");
    }

    void Engine::run()
    {
        m_input->update();
        m_world->update();

        Camera *camera = m_graphics->getCamera();
        float mapWidth = m_world->getTileMap()->getSizeX() * m_world->getTileMap()->getTileWidth();
        float mapHeight = m_world->getTileMap()->getSizeY() * m_world->getTileMap()->getTileHeight();
        camera->keepInBounds(0, 0, mapWidth - m_graphics->getVirtualWidth(), mapHeight - m_graphics->getVirtualHeight());
        camera->update();

        if (m_input->isPressed(Keyboard::Escape))
            m_running = false;

        m_graphics->clear(0.25, 0.25, 0.25, 1.0);
        m_world->render(m_graphics.get());

        text.clear();
        text.append("Draw Calls: ");
        text.append(std::to_string(m_graphics->drawCalls));
        text.append("\nCamera: ");
        text.append(std::to_string(camera->getX()));
        text.append(", ");
        text.append(std::to_string(camera->getY()));
        text.append("\nMouse:");
        text.append(std::to_string(m_input->getX()));
        text.append(", ");
        text.append(std::to_string(m_input->getY()));
        text.append("\n");
		
        m_graphics->setColor(1.0f, 1.0f, 1.0f, 1.0f);
        m_graphics->drawString(m_font.get(), text.c_str(), camera->getX() + 10, camera->getY() + (m_graphics->getVirtualHeight() - m_font->getLineHeight()) - 10);
        m_graphics->drawCalls = 0;

        m_graphics->submit();
    }

} // namespace Tektite
