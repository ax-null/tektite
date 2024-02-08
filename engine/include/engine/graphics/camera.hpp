#pragma once

#include <cstdlib>
#include <glm/glm.hpp>

namespace paper
{

    class Camera
    {
      public:
        Camera();
        ~Camera() = default;

        void update();

        void shake(float power)
        {
            m_shake_x = ((rand() % 2) - 1) * power;
            m_shake_y = ((rand() % 2) - 1) * power;
        }

        void zoom(float amount) { m_zoom += amount; }
        void rotate(float amount) { m_rotation += amount; }

        void move(float amountX, float amountY)
        {
            m_x += amountX;
            m_y += amountY;
        }

        void keep_in_bounds(float x, float y, float width, float height);

        void lerp_to(float x, float y, float amount)
        {
            m_x = m_x * (1.0 - amount) + (x * amount);
            m_y = m_y * (1.0 - amount) + (y * amount);
        }

        void set_position(float x, float y)
        {
            m_x = x;
            m_y = y;
        }

        const glm::mat4 &get_matrix() const { return m_transform; }

        float get_x() const { return m_x; }
        float get_y() const { return m_y; }
        float get_zoom() const { return m_zoom; }
        float get_rotation() const { return m_rotation; }

      private:
        float m_x, m_y, m_zoom, m_rotation, m_shake_x, m_shake_y;
        glm::mat4 m_transform;
    };
} // namespace paper
