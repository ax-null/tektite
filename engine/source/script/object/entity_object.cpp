#include <engine/script/script_engine.hpp>
#include <engine/world/entity.hpp>
#include <engine/world/world.hpp>

#include <new>

namespace paper
{

    class LuaEntity : public Entity
    {
      public:
        LuaEntity(lua_State *L, uint32_t tag, const char *name, float x, float y, float width, float height, World *world)
            : Entity(tag, name, x, y, width, height, world)
        {
            m_state = L;
        }

        ~LuaEntity()
        {
            luaL_unref(m_state, LUA_REGISTRYINDEX, m_ref);
        }

        void update() override
        {
            lua_rawgeti(m_state, LUA_REGISTRYINDEX, m_ref);
            lua_getfield(m_state, -1, "update");
            lua_pushvalue(m_state, -2);
            lua_pcall(m_state, 1, 0, 0);
        }

        void render() override
        {
            lua_rawgeti(m_state, LUA_REGISTRYINDEX, m_ref);
            lua_getfield(m_state, -1, "render");
            lua_pushvalue(m_state, -2);
            lua_pcall(m_state, 1, 0, 0);
        }

        void destroy() override
        {
            lua_rawgeti(m_state, LUA_REGISTRYINDEX, m_ref);
            lua_getfield(m_state, -1, "destroy");
            lua_pushvalue(m_state, -2);
            lua_pcall(m_state, 1, 0, 0);
        }

        void set_ref(int32_t ref) { m_ref = ref; }
        int32_t get_ref() const { return m_ref; }

      private:
        lua_State *m_state;
        int32_t m_ref;
    };

    static const luaL_Reg functions[] = {
        { "new", [](lua_State *L) {
             uint32_t tag = lua_tonumber(L, -10);
             const char *name = lua_tostring(L, -9);
             float x = lua_tonumber(L, -8);
             float y = lua_tonumber(L, -7);
             float width = lua_tonumber(L, -6);
             float height = lua_tonumber(L, -5);
             World *world = static_cast<World *>(lua_touserdata(L, -4));

             LuaEntity *entity = new (lua_newuserdatauv(L, sizeof(LuaEntity), 1))
                 LuaEntity(L, tag, name, x, y, width, height, world);

             world->add_entity(entity);

             luaL_getmetatable(L, "_entity_mt");
             lua_setmetatable(L, -2);

             lua_newtable(L);
             lua_setiuservalue(L, -2, 1);

             lua_pushvalue(L, -1);
             entity->set_ref(luaL_ref(L, LUA_REGISTRYINDEX));

             return 1;
         } },
        { "update", [](lua_State *L) {
             return 0;
         } },
        { "render", [](lua_State *L) {
             return 0;
         } },
        { "destroy", [](lua_State *L) {
             // TODO: Destory lua object
             return 0;
         } },
        { "collide_check", [](lua_State *L) {
             bool collide = false;
             if (lua_isuserdata(L, -3)) {
                 Entity *entity = (Entity *)lua_touserdata(L, -3);
                 float x = lua_tonumber(L, -2);
                 float y = lua_tonumber(L, -1);
                 collide = entity->collide_check(x, y);
             } else if (lua_isnumber(L, -3)) {
                 Entity *entity = (Entity *)lua_touserdata(L, -4);
                 uint32_t tag = lua_tonumber(L, -3);
                 float x = lua_tonumber(L, -2);
                 float y = lua_tonumber(L, -1);
                 collide = entity->collide_check(tag, x, y);
             }
             lua_pushboolean(L, collide);
             return 1;
         } },
        { "collide_first", [](lua_State *L) {
             LuaEntity *result = nullptr;
             if (lua_isuserdata(L, -3)) {
                 Entity *entity = (Entity *)lua_touserdata(L, -3);
                 float x = lua_tonumber(L, -2);
                 float y = lua_tonumber(L, -1);
                 result = (LuaEntity *)entity->collide_first(x, y);
             } else if (lua_isnumber(L, -3)) {
                 Entity *entity = (Entity *)lua_touserdata(L, -4);
                 uint32_t tag = lua_tonumber(L, -3);
                 float x = lua_tonumber(L, -2);
                 float y = lua_tonumber(L, -1);
                 result = (LuaEntity *)entity->collide_first(tag, x, y);
             }
             if (result != nullptr)
                 lua_rawgeti(L, LUA_REGISTRYINDEX, result->get_ref());
             else
                 lua_pushnil(L);
             return 1;
         } },
        { "set_position", [](lua_State *L) {
             Entity *entity = (Entity *)lua_touserdata(L, -3);
             float x = lua_tonumber(L, -2);
             float y = lua_tonumber(L, -1);

             entity->set_position(x, y);
             return 0;
         } },
        { "set_size", [](lua_State *L) {
             Entity *entity = (Entity *)lua_touserdata(L, -3);
             float width = lua_tonumber(L, -2);
             float height = lua_tonumber(L, -1);

             entity->set_size(width, height);
             return 0;
         } },
        { "get_tag", [](lua_State *L) {
             Entity *entity = (Entity *)lua_touserdata(L, -1);
             lua_pushnumber(L, entity->get_tag());
             return 1;
         } },
        { "get_name", [](lua_State *L) {
             Entity *entity = (Entity *)lua_touserdata(L, -1);
             lua_pushstring(L, entity->get_name());
             return 1;
         } },
        { "get_position", [](lua_State *L) {
             Entity *entity = (Entity *)lua_touserdata(L, -1);
             lua_pushnumber(L, entity->get_x());
             lua_pushnumber(L, entity->get_y());
             return 2;
         } },
        { "get_size", [](lua_State *L) {
             Entity *entity = (Entity *)lua_touserdata(L, -1);
             lua_pushnumber(L, entity->get_width());
             lua_pushnumber(L, entity->get_height());
             return 2;
         } },
        { nullptr, nullptr },
    };

    static int newindex(lua_State *L)
    {
        lua_getiuservalue(L, 1, 1);
        lua_pushvalue(L, 2);
        lua_pushvalue(L, 3);
        lua_settable(L, -3);
        return 0;
    }

    static int index(lua_State *L)
    {
        lua_getiuservalue(L, 1, 1);
        lua_pushvalue(L, 2);
        lua_gettable(L, -2);
        if (lua_isnil(L, -1)) {
            lua_getglobal(L, "entity");
            lua_pushvalue(L, 2);
            lua_rawget(L, -2);
        }
        return 1;
    }

    static int gc(lua_State *L)
    {
        Entity *entity = (Entity *)lua_touserdata(L, -1);
        entity->~Entity();
        return 0;
    }

    void object::define_entity(lua_State *L)
    {
        lua_newtable(L);
        lua_pushvalue(L, -1);
        luaL_setfuncs(L, functions, 1);
        lua_pushvalue(L, -2);
        lua_setfield(L, -2, "__index");
        lua_setglobal(L, "entity");

        luaL_newmetatable(L, "_entity_mt");
        lua_pushcfunction(L, gc);
        lua_setfield(L, -2, "__gc");
        lua_pushcfunction(L, newindex);
        lua_setfield(L, -2, "__newindex");
        lua_pushcfunction(L, index);
        lua_setfield(L, -2, "__index");
    }

} // namespace paper
