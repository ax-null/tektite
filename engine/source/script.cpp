#include <engine/script.hpp>

namespace paper
{

    Script::Script()
    {
        m_state = luaL_newstate();
        luaL_openlibs(m_state);
    }

    Script::~Script()
    {
        lua_close(m_state);
    }

    void Script::initialize_module(Audio *audio)
    {
        modules::initialize_audio(m_state, audio);
    }

    void Script::initialize_module(Graphics *graphics)
    {
        modules::initialize_graphics(m_state, graphics);
    }

    void Script::initialize_module(Input *input)
    {
        modules::initialize_input(m_state, input);
    }

    void Script::initialize_module(World *world)
    {
        modules::initialize_world(m_state, world);
    }

    void Script::define_objects()
    {
        objects::define_entity(m_state);
        objects::define_font(m_state);
        objects::define_texture(m_state);
    }

    void Script::execute(const char *path)
    {
        int status = luaL_dofile(m_state, path);
        if (status != LUA_OK)
            printf("Error: %s!\n", lua_tostring(m_state, -1));
    }

} // namespace paper
