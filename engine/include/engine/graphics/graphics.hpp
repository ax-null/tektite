#pragma once

#include "camera.hpp"
#include "font.hpp"
#include "texture.hpp"

#include <glm/glm.hpp>
#include <memory>
#include <stdint.h>

namespace paper
{

    class Graphics
    {
      public:
        Graphics(uint32_t virtualWidth, uint32_t virtualHeight);
        ~Graphics();

        void clear(float red, float green, float blue, float a);
        void resize(uint32_t width, uint32_t height);
        void submit();

        void set_color(float red, float green, float blue, float alpha);

        void draw_point(float x, float y);
        void draw_line(float x1, float y1, float x2, float y2);
        void draw_triangle(float x1, float y1, float x2, float y2, float x3, float y3, bool fill);
        void draw_rect(float x, float y, float width, float height, bool fill);
        void draw_circle(float x, float y, float radius, bool fill);
        void draw_texture(const Texture *texture, float x, float y, float width = -1, float height = -1,
                          float sourceX = 0, float sourceY = 0, float sourceWidth = -1, float sourceHeight = -1,
                          float rotation = 0, bool flipX = false, bool flipY = false);
        void draw_string(const Font *font, const char *text, float x, float y);
        void draw_tiles(const Texture *texture, const uint32_t *indices, uint32_t tileWidth, uint32_t tileHeight, uint32_t sizeX, uint32_t sizeY, float x, float y);

        Camera *get_camera() const { return m_camera.get(); }

        float get_screen_width() const { return m_screen_width; }
        float get_screen_height() const { return m_screen_height; }
        float get_virtual_width() const { return m_virtual_width; }
        float get_virtual_height() const { return m_virtual_height; }

        uint32_t draw_calls;

      private:
        static const uint32_t COUNT = 1000;
        static const uint32_t VERTEX_COUNT = COUNT * 8;
        static const uint32_t INDEX_COUNT = COUNT * 6;

        uint32_t m_program, m_tex, m_vao, m_vbo, m_ebo, m_mode;
        uint32_t m_texture, m_vertex_count, m_index_count, m_vertex_index;

        float m_screen_width, m_screen_height, m_virtual_width, m_virtual_height;
        float m_viewport_x, m_viewport_y, m_viewport_width, m_viewport_height;
        float m_red, m_green, m_blue, m_alpha;

        std::unique_ptr<Camera> m_camera;
        glm::mat4 m_projection;

        float m_vertices[VERTEX_COUNT];
        int32_t m_indices[INDEX_COUNT];
    };

} // namespace paper
