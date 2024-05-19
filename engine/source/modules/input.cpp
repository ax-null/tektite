#include <engine/input.hpp>

#include <engine/script.hpp>

namespace paper
{

    const luaL_Reg functions[] = {
        { "is_key_down", [](lua_State *L) {
             uint32_t key = static_cast<uint32_t>(lua_tonumber(L, -1));
             lua_getupvalue(L, -2, 1);
             Input *input = (Input *)lua_touserdata(L, -1);
             bool result = input->is_down(static_cast<keyboard::Key>(key));
             lua_pushboolean(L, result);
             return 1;
         } },
        { "is_key_pressed", [](lua_State *L) {
             uint32_t key = static_cast<uint32_t>(lua_tonumber(L, -1));
             lua_getupvalue(L, -2, 1);
             Input *input = (Input *)lua_touserdata(L, -1);
             bool result = input->is_pressed(static_cast<keyboard::Key>(key));
             lua_pushboolean(L, result);
             return 1;
         } },
        { "is_key_released", [](lua_State *L) {
             uint32_t key = static_cast<uint32_t>(lua_tonumber(L, -1));
             lua_getupvalue(L, -2, 1);
             Input *input = (Input *)lua_touserdata(L, -1);
             bool result = input->is_released(static_cast<keyboard::Key>(key));
             lua_pushboolean(L, result);
             return 1;
         } },
        { "is_key_held", [](lua_State *L) {
             uint32_t key = static_cast<uint32_t>(lua_tonumber(L, -1));
             lua_getupvalue(L, -2, 1);
             Input *input = (Input *)lua_touserdata(L, -1);
             bool result = input->is_held(static_cast<keyboard::Key>(key));
             lua_pushboolean(L, result);
             return 1;
         } },
        { "get_mouse_position", [](lua_State *L) {
             lua_getupvalue(L, -1, 1);
             Input *input = (Input *)lua_touserdata(L, -1);
             lua_pushnumber(L, input->get_x());
             lua_pushnumber(L, input->get_y());
             return 2;
         } },
        { "get_mouse_delta_position", [](lua_State *L) {
             lua_getupvalue(L, -1, 1);
             Input *input = (Input *)lua_touserdata(L, -1);
             lua_pushnumber(L, input->get_delta_x());
             lua_pushnumber(L, input->get_delta_y());
             return 2;
         } },
        { "is_mouse_button_down", [](lua_State *L) {
             uint32_t button = static_cast<uint32_t>(lua_tonumber(L, -1));
             lua_getupvalue(L, -2, 1);
             Input *input = (Input *)lua_touserdata(L, -1);
             bool result = input->is_down(static_cast<mouse::Button>(button));
             lua_pushboolean(L, result);
             return 1;
         } },
        { "is_mouse_button_pressed", [](lua_State *L) {
             uint32_t button = static_cast<uint32_t>(lua_tonumber(L, -1));
             lua_getupvalue(L, -2, 1);
             Input *input = (Input *)lua_touserdata(L, -1);
             bool result = input->is_pressed(static_cast<mouse::Button>(button));
             lua_pushboolean(L, result);
             return 1;
         } },
        { "is_mouse_button_released", [](lua_State *L) {
             uint32_t button = static_cast<uint32_t>(lua_tonumber(L, -1));
             lua_getupvalue(L, -2, 1);
             Input *input = (Input *)lua_touserdata(L, -1);
             bool result = input->is_released(static_cast<mouse::Button>(button));
             lua_pushboolean(L, result);
             return 1;
         } },
        { "is_mouse_button_held", [](lua_State *L) {
             uint32_t button = static_cast<uint32_t>(lua_tonumber(L, -1));
             lua_getupvalue(L, -2, 1);
             Input *input = (Input *)lua_touserdata(L, -1);
             bool result = input->is_held(static_cast<mouse::Button>(button));
             lua_pushboolean(L, result);
             return 1;
         } },
        { nullptr, nullptr },
    };

    void modules::initialize_input(lua_State *L, Input *input)
    {
        lua_newtable(L);
        lua_pushlightuserdata(L, input);
        luaL_setfuncs(L, functions, 1);
        lua_pushvalue(L, -2);
        lua_setfield(L, -2, "__index");
        lua_setglobal(L, "input");

        // Key definition for lua
        luaL_dostring(L, R"(
            KEY_UP = 0
            KEY_DOWN = 1
            KEY_LEFT = 2
            KEY_RIGHT = 3
            KEY_ESCAPE = 4
            KEY_ENTER = 5
            KEY_BACKSPACE = 6
            KEY_HOME = 7
            KEY_INSERT = 8
            KEY_PAGEUP = 9 
            KEY_PAGEDOWN = 10
            KEY_DELETE = 11
            KEY_END = 12
            KEY_PRINTSCREEN = 13
            KEY_PAUSE = 14
            KEY_SCROLLLOCK = 15
            KEY_NUMLOCK = 16
            KEY_CAPSLOCK = 17
            KEY_TAB = 18
            KEY_BACKSLASH = 19
            KEY_SPACE = 20
            KEY_LEFTSHIFT = 21
            KEY_RIGHTSHIFT = 22
            KEY_LEFTCTRL = 23
            KEY_RIGHTCTRL = 24
            KEY_LEFTALT = 25
            KEY_RIGHTALT = 26
            KEY_NUMPAD0 = 27
            KEY_NUMPAD1 = 28
            KEY_NUMPAD2 = 29
            KEY_NUMPAD3 = 30
            KEY_NUMPAD4 = 31
            KEY_NUMPAD5 = 32
            KEY_NUMPAD6 = 33
            KEY_NUMPAD7 = 34
            KEY_NUMPAD8 = 35
            KEY_NUMPAD9 = 36
            KEY_A = 37
            KEY_B = 38
            KEY_C = 39
            KEY_D = 40
            KEY_E = 41
            KEY_F = 42
            KEY_G = 43
            KEY_H = 44
            KEY_I = 45
            KEY_J = 46
            KEY_K = 47
            KEY_L = 48
            KEY_M = 49
            KEY_N = 50
            KEY_O = 51
            KEY_P = 52
            KEY_Q = 53
            KEY_R = 54
            KEY_S = 55
            KEY_T = 56
            KEY_U = 57
            KEY_V = 58
            KEY_W = 59
            KEY_X = 60
            KEY_Y = 61
            KEY_Z = 62
            KEY_NUM0 = 63
            KEY_NUM1 = 64
            KEY_NUM2 = 65
            KEY_NUM3 = 66
            KEY_NUM4 = 67
            KEY_NUM5 = 68
            KEY_NUM6 = 69
            KEY_NUM7 = 70
            KEY_NUM8 = 71
            KEY_NUM9 = 72
            KEY_F1 = 73
            KEY_F2 = 74
            KEY_F3 = 75
            KEY_F4 = 76
            KEY_F5 = 77
            KEY_F6 = 78
            KEY_F7 = 79
            KEY_F8 = 80
            KEY_F9 = 81
            KEY_F10 = 82
            KEY_F11 = 83
            KEY_F12 = 84
            KEY_UNKNOWN = 85

            MOUSE_BUTTON_LEFT = 0
            MOUSE_BUTTON_MIDDLE = 1
            MOUSE_BUTTON_RIGHT = 2
            MOUSE_BUTTON_UNKNOWN = 3
        )");
    }

} // namespace paper
