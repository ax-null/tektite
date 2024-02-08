#include <engine/script/script_engine.hpp>

namespace paper
{

    const luaL_Reg functions[] = {
        { "_", [](lua_State *L) {
             const char *path = static_cast<const char *>(lua_tostring(L, -1));
             lua_getupvalue(L, -2, 1);
             World *world = (World *)lua_touserdata(L, -1);
             return 0;
         } },
        { nullptr, nullptr }
    };

    void module::initialize_world(lua_State *L, World *world)
    {
        lua_newtable(L);
        lua_pushlightuserdata(L, world);
        luaL_setfuncs(L, functions, 1);
        lua_pushvalue(L, -2);
        lua_setfield(L, -2, "__index");
        lua_setglobal(L, "world");
    }

} // namespace paper
