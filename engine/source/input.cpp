#include <engine/input.hpp>

namespace paper
{

    void Input::update()
    {
        m_prev_keys = m_curr_keys;
        m_prev_mouse = m_curr_mouse;
        m_curr_keys = m_keys;
        m_curr_mouse = m_mouse;
    }

    void Input::on_move(float x, float y)
    {
        m_mouse.posX = x;
        m_mouse.posY = y;
    }

    void Input::on_press(keyboard::Key key)
    {
        m_keys.keys[static_cast<uint32_t>(key)] = true;
    }

    void Input::on_press(mouse::Button button)
    {
        m_mouse.buttons[static_cast<uint32_t>(button)] = true;
    }

    void Input::on_release(keyboard::Key key)
    {
        m_keys.keys[static_cast<uint32_t>(key)] = false;
    }

    void Input::on_release(mouse::Button button)
    {
        m_mouse.buttons[static_cast<uint32_t>(button)] = false;
    }

    float Input::get_x() const
    {
        return m_curr_mouse.posX;
    }

    float Input::get_y() const
    {
        return m_curr_mouse.posY;
    }

    float Input::get_delta_x() const
    {
        return m_curr_mouse.posX - m_prev_mouse.posX;
    }

    float Input::get_delta_y() const
    {
        return m_curr_mouse.posY - m_prev_mouse.posY;
    }

    bool Input::is_down(keyboard::Key key) const
    {
        return m_curr_keys.keys[static_cast<uint32_t>(key)];
    }

    bool Input::is_down(mouse::Button button) const
    {
        return m_curr_mouse.buttons[static_cast<uint32_t>(button)];
    }

    bool Input::is_pressed(keyboard::Key key) const
    {
        uint32_t key_index = static_cast<uint32_t>(key);
        return m_curr_keys.keys[key_index] && !m_prev_keys.keys[key_index];
    }

    bool Input::is_pressed(mouse::Button button) const
    {
        uint32_t button_index = static_cast<uint32_t>(button);
        return m_curr_mouse.buttons[button_index] && !m_prev_mouse.buttons[button_index];
    }

    bool Input::is_released(keyboard::Key key) const
    {
        uint32_t key_index = static_cast<uint32_t>(key);
        return m_prev_keys.keys[key_index] && !m_curr_keys.keys[key_index];
    }

    bool Input::is_released(mouse::Button button) const
    {
        uint32_t button_index = static_cast<uint32_t>(button);
        return m_prev_mouse.buttons[button_index] && !m_curr_mouse.buttons[button_index];
    }

    bool Input::is_held(keyboard::Key key) const
    {
        uint32_t key_index = static_cast<uint32_t>(key);
        return m_curr_keys.keys[key_index] && m_prev_keys.keys[key_index];
    }

    bool Input::is_held(mouse::Button button) const
    {
        uint32_t button_index = static_cast<uint32_t>(button);
        return m_curr_mouse.buttons[button_index] && m_prev_mouse.buttons[button_index];
    }

} // namespace paper
