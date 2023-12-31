#pragma once

#include <cstdlib>
#include <glm/glm.hpp>

namespace Tektite
{

    class Camera
    {
      public:
        Camera();
        ~Camera() = default;

        void update();

        void shake(float power)
        {
            m_shakeX = ((rand() % 2) - 1) * power;
            m_shakeY = ((rand() % 2) - 1) * power;
        }

        void zoom(float amount) { m_zoom += amount; }
        void rotate(float amount) { m_rotation += amount; }

        void move(float amountX, float amountY)
        {
            m_x += amountX;
            m_y += amountY;
        }

        void keepInBounds(float x, float y, float width, float height);

        void lerpTo(float x, float y, float amount)
        {
            m_x = m_x * (1.0 - amount) + (x * amount);
            m_y = m_y * (1.0 - amount) + (y * amount);
        }

        void setPosition(float x, float y)
        {
            m_x = x;
            m_y = y;
        }

        const glm::mat4 &getMatrix() const { return m_transform; }

        float getX() const { return m_x; }
        float getY() const { return m_y; }
        float getZoom() const { return m_zoom; }
        float getRotation() const { return m_rotation; }

      private:
        float m_x, m_y, m_zoom, m_rotation, m_shakeX, m_shakeY;
        glm::mat4 m_transform;
    };
} // namespace Tektite
