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
        void drawTexture(Texture *texture, float x, float y, float width = -1, float height = -1,
                         float sourceX = 0, float sourceY = 0, float sourceWidth = -1, float sourceHeight = -1,
                         float rotation = 0, bool flipX = false, bool flipY = false);
        void drawString(Font *font, const char *text, float x, float y);
    };

} // namespace Tektite
