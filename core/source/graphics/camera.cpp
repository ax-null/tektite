#include "glm/common.hpp"
#include <core/graphics/camera.hpp>

#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Tektite
{
    Camera::Camera()
    {
        m_transform = glm::identity<glm::mat4>();
        m_x = m_y = m_rotation = m_shakeX = m_shakeY = 0;
        m_zoom = 1;
    }

    void Camera::update()
    {
        m_transform = glm::identity<glm::mat4>();
        m_transform = glm::lookAt(glm::vec3(m_x + m_shakeX, m_y + m_shakeY, 0), glm::vec3(m_x + m_shakeX, m_y + m_shakeY, -1.0f), glm::vec3(0, 1.0f, 0));
        m_shakeX = m_shakeY = 0;
    }

    void Camera::keepInBounds(float x, float y, float width, float height)
    {
        m_x = glm::clamp(m_x, x, x + width);
        m_y = glm::clamp(m_y, y, y + height);
    }

} // namespace Tektite
