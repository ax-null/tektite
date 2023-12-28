#include <core/engine/engine.hpp>

namespace Tektite
{

    Engine::Engine()
    {
        m_running = true;

        m_audio = std::make_unique<Audio>();
        m_graphics = std::make_unique<Graphics>(800, 600);
        m_input = std::make_unique<Input>();
        m_script = std::make_unique<Script>();

        m_script->initializeModule(m_audio.get());
        m_script->initializeModule(m_graphics.get());
        m_script->initializeModule(m_input.get());

        m_script->initializeObjects();

        m_script->execute("assets/scripts/init.lua");
    }

    void Engine::run()
    {
        m_input->update();

        if (m_input->isPressed(Keyboard::Escape))
            m_running = false;

        lua_getglobal(m_script->getState(), "Loop");
        lua_pcall(m_script->getState(), 0, 0, 0);
    }

} // namespace Tektite
