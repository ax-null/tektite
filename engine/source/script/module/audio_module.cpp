#include <engine/audio/audio_engine.hpp>
#include <engine/script/script_engine.hpp>

namespace tektite
{
    const luaL_Reg functions[] = {
        { "play_sound", [](lua_State *L) {
             const char *path = static_cast<const char *>(lua_tostring(L, -1));
             lua_getupvalue(L, -2, 1);
             AudioEngine *audio = (AudioEngine *)lua_touserdata(L, -1);
             audio->play_sound(path);
             return 0;
         } },
        { nullptr, nullptr }
    };

    void module::initialize_audio_engine(lua_State *L, AudioEngine *audio)
    {
        lua_newtable(L);
        lua_pushlightuserdata(L, audio);
        luaL_setfuncs(L, functions, 1);
        lua_pushvalue(L, -2);
        lua_setfield(L, -2, "__index");
        lua_setglobal(L, "audio");
    }

} // namespace tektite
