#include <core/script/script.hpp>

namespace Tektite
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

    void Script::initializeModule(Audio *audio)
    {
        Module::initializeAudio(m_state, audio);
    }

    void Script::initializeModule(Graphics *graphics)
    {
        Module::initializeGraphics(m_state, graphics);
    }

    void Script::initializeModule(Input *input)
    {
        Module::initializeInput(m_state, input);
    }

    void Script::initializeObjects()
    {
        Object::defineFont(m_state);
        Object::defineTexture(m_state);
    }

    void Script::execute(const char *path)
    {
        int status = luaL_dofile(m_state, path);
        if (status != LUA_OK)
            printf("Error: %s!\n", lua_tostring(m_state, -1));
    }

} // namespace Tektite
