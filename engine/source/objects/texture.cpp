#include <engine/graphics/texture.hpp>

#include <engine/script.hpp>

#include <new>

namespace paper
{

    static const luaL_Reg functions[] = {
        { "new", [](lua_State *L) {
             const char *path = lua_tostring(L, -1);
             new (lua_newuserdata(L, sizeof(Texture))) Texture(path);
             luaL_getmetatable(L, "_texture_mt");
             lua_setmetatable(L, -2);
             return 1;
         } },
        { nullptr, nullptr },
    };

    static int gc(lua_State *L)
    {
        Texture *texture = (Texture *)lua_touserdata(L, -1);
        texture->~Texture();
        return 0;
    }

    void objects::define_texture(lua_State *L)
    {
        lua_newtable(L);
        int table_index = lua_gettop(L);
        lua_pushvalue(L, table_index);

        luaL_setfuncs(L, functions, 0);
        lua_pushvalue(L, -2);
        lua_setfield(L, -2, "__index");
        lua_setglobal(L, "texture");

        luaL_newmetatable(L, "_texture_mt");
        lua_pushcfunction(L, gc);
        lua_setfield(L, -2, "__gc");
        lua_pushvalue(L, table_index);
        lua_setfield(L, -2, "__index");
    }

} // namespace paper
