#pragma once

#include "core/graphics/camera.hpp"
#include "font.hpp"
#include "texture.hpp"

#include <glm/glm.hpp>
#include <memory>
#include <stdint.h>

namespace Tektite
{
    class Graphics
    {
      public:
        Graphics(uint32_t virtualWidth, uint32_t virtualHeight);
        ~Graphics();

        void clear(float red, float green, float blue, float a);
        void resize(uint32_t width, uint32_t height);
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

        Camera *getCamera() const { return m_camera.get(); }

        float getScreenWidth() const { return m_screenWidth; }
        float getScreenHeight() const { return m_screenHeight; }
        float getVirtualWidth() const { return m_virtualWidth; }
        float getVirtualHeight() const { return m_virtualHeight; }

        uint32_t drawCalls;

      private:
        static const uint32_t COUNT = 1000;
        static const uint32_t VERTEX_COUNT = COUNT * 8;
        static const uint32_t INDEX_COUNT = COUNT * 6;

        uint32_t m_program, m_tex, m_vao, m_vbo, m_ebo, m_mode;
        uint32_t m_texture, m_vertex_count, m_index_count, m_vertex_index;

        float m_screenWidth, m_screenHeight, m_virtualWidth, m_virtualHeight;
        float m_viewportX, m_viewportY, m_viewportWidth, m_viewportHeight;
        float m_red, m_green, m_blue, m_alpha;

        std::unique_ptr<Camera> m_camera;
        glm::mat4 m_projection;

        float m_vertices[VERTEX_COUNT];
        int32_t m_indices[INDEX_COUNT];
    };

} // namespace Tektite
