#include "core/graphics/font.hpp"
#include <core/script/script.hpp>

#include <new>

namespace Tektite
{
    static const luaL_Reg functions[] = {
        { "new", [](lua_State *L) {
             const char *path = lua_tostring(L, -1);
             new (lua_newuserdata(L, sizeof(Font))) Font(path);
             luaL_getmetatable(L, "_font_mt");
             lua_setmetatable(L, -2);
             return 1;
         } },
        { nullptr, nullptr },
    };

    static int gc(lua_State *L)
    {
        Font *font = (Font *)lua_touserdata(L, -1);
        font->~Font();
        return 0;
    }

    void Object::defineFont(lua_State *L)
    {
        lua_newtable(L);
        int tableIndex = lua_gettop(L);
        lua_pushvalue(L, tableIndex);

        luaL_setfuncs(L, functions, 0);
        lua_pushvalue(L, -2);
        lua_setfield(L, -2, "__index");
        lua_setglobal(L, "font");

        luaL_newmetatable(L, "_font_mt");
        lua_pushcfunction(L, gc);
        lua_setfield(L, -2, "__gc");
        lua_pushvalue(L, tableIndex);
        lua_setfield(L, -2, "__index");
    }
} // namespace Tektite
