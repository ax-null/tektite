#pragma once

extern "C" {
#include <lauxlib.h>
#include <lua.h>
#include <lualib.h>
}

namespace paper
{

    class Audio;
    class Graphics;
    class Input;
    class World;

    namespace modules
    {

        void initialize_audio(lua_State *L, Audio *audio);
        void initialize_graphics(lua_State *L, Graphics *graphics);
        void initialize_input(lua_State *L, Input *input);
        void initialize_world(lua_State *L, World *world);

    } // namespace module

    namespace objects
    {

        void define_entity(lua_State *L);
        void define_font(lua_State *L);
        void define_texture(lua_State *L);

    } // namespace object

    class Script
    {
      public:
        Script();
        ~Script();

        void initialize_module(Audio *audio);
        void initialize_module(Graphics *graphics);
        void initialize_module(Input *input);
        void initialize_module(World *world);

        void define_objects();

        void execute(const char *path);

        lua_State *get_state() const { return m_state; }

      private:
        lua_State *m_state;
    };

} // namespace paper
