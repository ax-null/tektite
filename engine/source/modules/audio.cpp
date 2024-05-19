#include <engine/audio.hpp>

#include <engine/script.hpp>

namespace paper
{

    const luaL_Reg functions[] = {
        { "play_sound", [](lua_State *L) {
             const char *path = static_cast<const char *>(lua_tostring(L, -1));
             lua_getupvalue(L, -2, 1);
             Audio *audio = (Audio *)lua_touserdata(L, -1);
             audio->play_sound(path);
             return 0;
         } },
        { nullptr, nullptr }
    };

    void modules::initialize_audio(lua_State *L, Audio *audio)
    {
        lua_newtable(L);
        lua_pushlightuserdata(L, audio);
        luaL_setfuncs(L, functions, 1);
        lua_pushvalue(L, -2);
        lua_setfield(L, -2, "__index");
        lua_setglobal(L, "audio");
    }

} // namespace paper
