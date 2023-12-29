#pragma once

extern "C" {
#include <lauxlib.h>
#include <lua.h>
#include <lualib.h>
}

namespace Tektite
{

    class Audio;
    class Graphics;
    class Input;
    class World;

    namespace Module
    {
        void initializeAudio(lua_State *L, Audio *audio);
        void initializeGraphics(lua_State *L, Graphics *graphics);
        void initializeInput(lua_State *L, Input *input);
        void initializeWorld(lua_State *L, World *world);
    } // namespace Module

    namespace Object
    {
        void defineEntity(lua_State *L);
        void defineFont(lua_State *L);
        void defineTexture(lua_State *L);
    } // namespace Object

    class Script
    {
      public:
        Script();
        ~Script();

        void initializeModule(Audio *audio);
        void initializeModule(Graphics *graphics);
        void initializeModule(Input *input);
        void initializeModule(World *world);

        void defineObjects();

        void execute(const char *path);

        lua_State *getState() const { return m_state; }

      private:
        lua_State *m_state;
    };
} // namespace Tektite
