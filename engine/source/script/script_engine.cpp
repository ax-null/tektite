#include <engine/script/script_engine.hpp>

namespace tektite
{

    ScriptEngine::ScriptEngine()
    {
        m_state = luaL_newstate();
        luaL_openlibs(m_state);
    }

    ScriptEngine::~ScriptEngine()
    {
        lua_close(m_state);
    }

    void ScriptEngine::initialize_module(AudioEngine *audio)
    {
        module::initialize_audio_engine(m_state, audio);
    }

    void ScriptEngine::initialize_module(Graphics *graphics)
    {
        module::initialize_graphics(m_state, graphics);
    }

    void ScriptEngine::initialize_module(InputSystem *input)
    {
        module::initialize_input_system(m_state, input);
    }

    void ScriptEngine::initialize_module(World *world)
    {
        module::initialize_world(m_state, world);
    }

    void ScriptEngine::define_objects()
    {
        object::define_entity(m_state);
        object::define_font(m_state);
        object::define_texture(m_state);
    }

    void ScriptEngine::execute(const char *path)
    {
        int status = luaL_dofile(m_state, path);
        if (status != LUA_OK)
            printf("Error: %s!\n", lua_tostring(m_state, -1));
    }

} // namespace tektite
