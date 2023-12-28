#include <core/input/input.hpp>

namespace Tektite
{

    void Input::update()
    {
        m_prevKeys = m_currKeys;
        m_prevMouse = m_currMouse;
        m_currKeys = m_keys;
        m_currMouse = m_mouse;
    }

    void Input::onMove(float x, float y)
    {
        m_mouse.posX = x;
        m_mouse.posY = y;
    }

    void Input::onPress(Keyboard::Key key)
    {
        m_keys.keys[static_cast<uint32_t>(key)] = true;
    }

    void Input::onPress(Mouse::Button button)
    {
        m_mouse.buttons[static_cast<uint32_t>(button)] = true;
    }

    void Input::onRelease(Keyboard::Key key)
    {
        m_keys.keys[static_cast<uint32_t>(key)] = false;
    }

    void Input::onRelease(Mouse::Button button)
    {
        m_mouse.buttons[static_cast<uint32_t>(button)] = false;
    }

    float Input::getX() const
    {
        return m_currMouse.posX;
    }

    float Input::getY() const
    {
        return m_currMouse.posY;
    }

    float Input::getDeltaX() const
    {
        return m_currMouse.posX - m_prevMouse.posX;
    }

    float Input::getDeltaY() const
    {
        return m_currMouse.posY - m_prevMouse.posY;
    }

    bool Input::isDown(Keyboard::Key key) const
    {
        return m_currKeys.keys[static_cast<uint32_t>(key)];
    }

    bool Input::isDown(Mouse::Button button) const
    {
        return m_currMouse.buttons[static_cast<uint32_t>(button)];
    }

    bool Input::isPressed(Keyboard::Key key) const
    {
        uint32_t keyIndex = static_cast<uint32_t>(key);
        return m_currKeys.keys[keyIndex] && !m_prevKeys.keys[keyIndex];
    }

    bool Input::isPressed(Mouse::Button button) const
    {
        uint32_t buttonIndex = static_cast<uint32_t>(button);
        return m_currMouse.buttons[buttonIndex] && !m_prevMouse.buttons[buttonIndex];
    }

    bool Input::isReleased(Keyboard::Key key) const
    {
        uint32_t keyIndex = static_cast<uint32_t>(key);
        return m_prevKeys.keys[keyIndex] && !m_currKeys.keys[keyIndex];
    }

    bool Input::isReleased(Mouse::Button button) const
    {
        uint32_t buttonIndex = static_cast<uint32_t>(button);
        return m_prevMouse.buttons[buttonIndex] && !m_currMouse.buttons[buttonIndex];
    }

    bool Input::isHeld(Keyboard::Key key) const
    {
        uint32_t keyIndex = static_cast<uint32_t>(key);
        return m_currKeys.keys[keyIndex] && m_prevKeys.keys[keyIndex];
    }

    bool Input::isHeld(Mouse::Button button) const
    {
        uint32_t buttonIndex = static_cast<uint32_t>(button);
        return m_currMouse.buttons[buttonIndex] && m_prevMouse.buttons[buttonIndex];
    }

} // namespace Tektite
