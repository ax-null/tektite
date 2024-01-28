#pragma once

extern "C" {
#include <lauxlib.h>
#include <lua.h>
#include <lualib.h>
}

namespace tektite
{

    class AudioEngine;
    class Graphics;
    class InputSystem;
    class World;

    namespace module
    {
        void initialize_audio_engine(lua_State *L, AudioEngine *audio);
        void initialize_graphics(lua_State *L, Graphics *graphics);
        void initialize_input_system(lua_State *L, InputSystem *input);
        void initialize_world(lua_State *L, World *world);
    } // namespace Module

    namespace object
    {
        void define_entity(lua_State *L);
        void define_font(lua_State *L);
        void define_texture(lua_State *L);
    } // namespace Object

    class ScriptEngine
    {
      public:
        ScriptEngine();
        ~ScriptEngine();

        void initialize_module(AudioEngine *audio);
        void initialize_module(Graphics *graphics);
        void initialize_module(InputSystem *input);
        void initialize_module(World *world);

        void define_objects();

        void execute(const char *path);

        lua_State *get_state() const { return m_state; }

      private:
        lua_State *m_state;
    };
} // namespace tektite
