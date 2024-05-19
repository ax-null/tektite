#include <engine/graphics.hpp>

#include <engine/script.hpp>

namespace paper
{

    static const luaL_Reg functions[] = {
        { "clear", [](lua_State *L) {
             float red = static_cast<float>(lua_tonumber(L, -4));
             float green = static_cast<float>(lua_tonumber(L, -3));
             float blue = static_cast<float>(lua_tonumber(L, -2));
             float alpha = static_cast<float>(lua_tonumber(L, -1));
             lua_getupvalue(L, -5, 1);
             Graphics *graphics = (Graphics *)lua_touserdata(L, -1);
             graphics->clear(red, green, blue, alpha);
             return 0;
         } },
        { "submit", [](lua_State *L) {
             lua_getupvalue(L, -1, 1);
             Graphics *graphics = (Graphics *)lua_touserdata(L, -1);
             graphics->submit();
             return 0;
         } },
        { "set_color", [](lua_State *L) {
             float red = static_cast<float>(lua_tonumber(L, -4));
             float green = static_cast<float>(lua_tonumber(L, -3));
             float blue = static_cast<float>(lua_tonumber(L, -2));
             float alpha = static_cast<float>(lua_tonumber(L, -1));
             lua_getupvalue(L, -5, 1);
             Graphics *graphics = (Graphics *)lua_touserdata(L, -1);
             graphics->set_color(red, green, blue, alpha);
             return 0;
         } },
        { "draw_point", [](lua_State *L) {
             float x = static_cast<float>(lua_tonumber(L, -2));
             float y = static_cast<float>(lua_tonumber(L, -1));
             lua_getupvalue(L, -3, 1);
             Graphics *graphics = (Graphics *)lua_touserdata(L, -1);
             graphics->draw_point(x, y);
             return 0;
         } },
        { "draw_line", [](lua_State *L) {
             float x1 = static_cast<float>(lua_tonumber(L, -4));
             float y1 = static_cast<float>(lua_tonumber(L, -3));
             float x2 = static_cast<float>(lua_tonumber(L, -2));
             float y2 = static_cast<float>(lua_tonumber(L, -1));
             lua_getupvalue(L, -5, 1);
             Graphics *graphics = (Graphics *)lua_touserdata(L, -1);
             graphics->draw_line(x1, y1, x2, y2);
             return 0;
         } },
        { "draw_triangle", [](lua_State *L) {
             float x1 = static_cast<float>(lua_tonumber(L, -7));
             float y1 = static_cast<float>(lua_tonumber(L, -6));
             float x2 = static_cast<float>(lua_tonumber(L, -5));
             float y2 = static_cast<float>(lua_tonumber(L, -4));
             float x3 = static_cast<float>(lua_tonumber(L, -3));
             float y3 = static_cast<float>(lua_tonumber(L, -2));
             bool fill = static_cast<bool>(lua_toboolean(L, -1));
             lua_getupvalue(L, -8, 1);
             Graphics *graphics = (Graphics *)lua_touserdata(L, -1);
             graphics->draw_triangle(x1, y1, x2, y2, x3, y3, fill);
             return 0;
         } },
        { "draw_rect", [](lua_State *L) {
             float x = static_cast<float>(lua_tonumber(L, -5));
             float y = static_cast<float>(lua_tonumber(L, -4));
             float width = static_cast<float>(lua_tonumber(L, -3));
             float height = static_cast<float>(lua_tonumber(L, -2));
             bool fill = static_cast<bool>(lua_toboolean(L, -1));
             lua_getupvalue(L, -6, 1);
             Graphics *graphics = (Graphics *)lua_touserdata(L, -1);
             graphics->draw_rect(x, y, width, height, fill);
             return 0;
         } },
        { "draw_circle", [](lua_State *L) {
             float x = static_cast<float>(lua_tonumber(L, -4));
             float y = static_cast<float>(lua_tonumber(L, -3));
             float radius = static_cast<float>(lua_tonumber(L, -2));
             bool fill = static_cast<bool>(lua_toboolean(L, -1));
             lua_getupvalue(L, -5, 1);
             Graphics *graphics = (Graphics *)lua_touserdata(L, -1);
             graphics->draw_circle(x, y, radius, fill);
             return 0;
         } },
        { "draw_texture", [](lua_State *L) {
             Texture *texture = static_cast<Texture *>(lua_touserdata(L, -3));
             float x = static_cast<float>(lua_tonumber(L, -2));
             float y = static_cast<float>(lua_tonumber(L, -1));
             lua_getupvalue(L, -4, 1);
             Graphics *graphics = (Graphics *)lua_touserdata(L, -1);
             graphics->draw_texture(texture, x, y);
             return 0;
         } },
        { "draw_string", [](lua_State *L) {
             Font *font = static_cast<Font *>(lua_touserdata(L, -4));
             const char *text = static_cast<const char *>(lua_tostring(L, -3));
             float x = static_cast<float>(lua_tonumber(L, -2));
             float y = static_cast<float>(lua_tonumber(L, -1));
             lua_getupvalue(L, -5, 1);
             Graphics *graphics = (Graphics *)lua_touserdata(L, -1);
             graphics->draw_string(font, text, x, y);
             return 0;
         } },
        { "set_camera_position", [](lua_State *L) {
             float x = static_cast<float>(lua_tonumber(L, -2));
             float y = static_cast<float>(lua_tonumber(L, -1));
             lua_getupvalue(L, -3, 1);
             Graphics *graphics = (Graphics *)lua_touserdata(L, -1);
             graphics->get_camera()->set_position(x, y);
             return 0;
         } },
        { "set_camera_lerp_to", [](lua_State *L) {
             float x = static_cast<float>(lua_tonumber(L, -3));
             float y = static_cast<float>(lua_tonumber(L, -2));
             float amount = static_cast<float>(lua_tonumber(L, -1));
             lua_getupvalue(L, -4, 1);
             Graphics *graphics = (Graphics *)lua_touserdata(L, -1);
             graphics->get_camera()->lerp_to(x, y, amount);
             return 0;
         } },
        { "get_camera_position", [](lua_State *L) {
             lua_getupvalue(L, -1, 1);
             Graphics *graphics = (Graphics *)lua_touserdata(L, -1);
             lua_pushnumber(L, graphics->get_camera()->get_x());
             lua_pushnumber(L, graphics->get_camera()->get_y());
             return 2;
         } },
        { "get_screen_size", [](lua_State *L) {
             lua_getupvalue(L, -1, 1);
             Graphics *graphics = (Graphics *)lua_touserdata(L, -1);
             lua_pushnumber(L, graphics->get_screen_width());
             lua_pushnumber(L, graphics->get_screen_height());
             return 2;
         } },
        { "get_virtual_size", [](lua_State *L) {
             lua_getupvalue(L, -1, 1);
             Graphics *graphics = (Graphics *)lua_touserdata(L, -1);
             lua_pushnumber(L, graphics->get_virtual_width());
             lua_pushnumber(L, graphics->get_virtual_height());
             return 2;
         } },
        { nullptr, nullptr }
    };

    void modules::initialize_graphics(lua_State *L, Graphics *graphics)
    {
        lua_newtable(L);
        lua_pushlightuserdata(L, graphics);
        luaL_setfuncs(L, functions, 1);
        lua_pushvalue(L, -2);
        lua_setfield(L, -2, "__index");
        lua_setglobal(L, "graphics");
    }

} // namespace paper
