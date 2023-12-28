#pragma once

extern "C" {
#include <lauxlib.h>
#include <lua.h>
#include <lualib.h>
}

#include "../audio/audio.hpp"
#include "../graphics/graphics.hpp"
#include "../input/input.hpp"

namespace Tektite
{

    namespace Module
    {
        void initializeAudio(lua_State *L, Audio *audio);
        void initializeGraphics(lua_State *L, Graphics *graphics);
        void initializeInput(lua_State *L, Input *input);
    } // namespace Module

    namespace Object
    {
        void defineFont(lua_State *L);
        void defineTexture(lua_State *L);
    }

    class Script
    {
      public:
        Script();
        ~Script();

        void initializeModule(Audio *audio);
        void initializeModule(Graphics *graphics);
        void initializeModule(Input *input);

        void initializeObjects();

        void execute(const char *path);

        lua_State *getState() const { return m_state; }

      private:
        lua_State *m_state;
    };
} // namespace Tektite
