#pragma once

#include "font.hpp"
#include "texture.hpp"

#include <glm/glm.hpp>
#include <stdint.h>

namespace Tektite
{
    class Graphics
    {
      public:
        Graphics(uint32_t width, uint32_t height);
        ~Graphics();

        void clear(float red, float green, float blue, float a);
        void submit();

        void setColor(float red, float green, float blue, float alpha);

        void drawPoint(float x, float y);
        void drawLine(float x1, float y1, float x2, float y2);
        void drawTriangle(float x1, float y1, float x2, float y2, float x3, float y3, bool fill);
        void drawRect(float x, float y, float width, float height, bool fill);
        void drawCircle(float x, float y, float radius, bool fill);
        void drawTexture(const Texture *texture, float x, float y, float width = -1, float height = -1,
                         float sourceX = 0, float sourceY = 0, float sourceWidth = -1, float sourceHeight = -1,
                         float rotation = 0, bool flipX = false, bool flipY = false);
        void drawString(const Font *font, const char *text, float x, float y);
        void drawTiles(const Texture *texture, const uint32_t *indices, uint32_t tileWidth, uint32_t tileHeight, uint32_t sizeX, uint32_t sizeY, float x, float y);

        uint32_t drawCalls;

      private:
        static const uint32_t COUNT = 1000;
        static const uint32_t VERTEX_COUNT = COUNT * 8;
        static const uint32_t INDEX_COUNT = COUNT * 6;

        uint32_t m_program, m_tex, m_vao, m_vbo, m_ebo, m_mode;
        uint32_t m_texture, m_vertex_count, m_index_count, m_vertex_index;

        float m_red, m_green, m_blue, m_alpha;

        glm::mat4 m_matrix;

        float m_vertices[VERTEX_COUNT];
        int32_t m_indices[INDEX_COUNT];
    };

} // namespace Tektite
