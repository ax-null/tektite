#include <core/script/script.hpp>

namespace Tektite
{
    const luaL_Reg functions[] = {
        { "_", [](lua_State *L) {
             const char* path = static_cast<const char*>(lua_tostring(L, -1));
             lua_getupvalue(L, -2, 1);
             World *world = (World *)lua_touserdata(L, -1);
             return 0;
         } },
        { nullptr, nullptr }
    };

    void Module::initializeWorld(lua_State *L, World *world)
    {
        lua_newtable(L);
        lua_pushlightuserdata(L, world);
        luaL_setfuncs(L, functions, 1);
        lua_pushvalue(L, -2);
        lua_setfield(L, -2, "__index");
        lua_setglobal(L, "world");
    }

} // namespace Tektite
