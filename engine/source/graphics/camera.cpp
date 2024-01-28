#include <engine/graphics/camera.hpp>

#include <glm/common.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace tektite
{
    Camera::Camera()
    {
        m_transform = glm::identity<glm::mat4>();
        m_x = m_y = m_rotation = m_shake_x = m_shake_y = 0;
        m_zoom = 1;
    }

    void Camera::update()
    {
        m_transform = glm::identity<glm::mat4>();
        m_transform = glm::lookAt(glm::vec3(m_x + m_shake_x, m_y + m_shake_y, 0), glm::vec3(m_x + m_shake_x, m_y + m_shake_y, -1.0f), glm::vec3(0, 1.0f, 0));
        m_shake_x = m_shake_y = 0;
    }

    void Camera::keep_in_bounds(float x, float y, float width, float height)
    {
        m_x = glm::clamp(m_x, x, x + width);
        m_y = glm::clamp(m_y, y, y + height);
    }

} // namespace tektite
