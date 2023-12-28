#pragma once

#include <stdint.h>

namespace Tektite
{
    namespace Keyboard
    {
        enum Key
        {
            Up,
            Down,
            Left,
            Right,
            Escape,
            Enter,
            Backspace,
            Home,
            Insert,
            PageUp,
            PageDown,
            Delete,
            End,
            PrintScreen,
            Pause,
            ScrollLock,
            NumLock,
            CapsLock,
            Tab,
            Backslash,
            Space,
            LeftShift,
            RightShift,
            LeftCtrl,
            RightCtrl,
            LeftAlt,
            RightAlt,
            Numpad0,
            Numpad1,
            Numpad2,
            Numpad3,
            Numpad4,
            Numpad5,
            Numpad6,
            Numpad7,
            Numpad8,
            Numpad9,
            A,
            B,
            C,
            D,
            E,
            F,
            G,
            H,
            I,
            J,
            K,
            L,
            M,
            N,
            O,
            P,
            Q,
            R,
            S,
            T,
            U,
            V,
            W,
            X,
            Y,
            Z,
            Num0,
            Num1,
            Num2,
            Num3,
            Num4,
            Num5,
            Num6,
            Num7,
            Num8,
            Num9,
            F1,
            F2,
            F3,
            F4,
            F5,
            F6,
            F7,
            F8,
            F9,
            F10,
            F11,
            F12,
            Unknown,
        };

        struct State
        {
            bool keys[static_cast<uint32_t>(Keyboard::Unknown)];
        };

    } // namespace Keyboard

    namespace Mouse
    {
        enum Button
        {
            Left,
            Middle,
            Right,
            Unknown,
        };

        struct State
        {
            float posX, posY;
            bool buttons[static_cast<uint32_t>(Mouse::Unknown)];
        };

    } // namespace Mouse

    class Input
    {
      public:
        Input() = default;
        ~Input() = default;

        void update();

        void onMove(float x, float y);

        void onPress(Keyboard::Key key);
        void onPress(Mouse::Button button);

        void onRelease(Keyboard::Key key);
        void onRelease(Mouse::Button button);

        float getX() const;
        float getY() const;

        float getDeltaX() const;
        float getDeltaY() const;

        bool isDown(Keyboard::Key key) const;
        bool isDown(Mouse::Button button) const;

        bool isPressed(Keyboard::Key key) const;
        bool isPressed(Mouse::Button button) const;

        bool isReleased(Keyboard::Key key) const;
        bool isReleased(Mouse::Button button) const;

        bool isHeld(Keyboard::Key key) const;
        bool isHeld(Mouse::Button button) const;

      private:
        Keyboard::State m_keys, m_prevKeys, m_currKeys;
        Mouse::State m_mouse, m_prevMouse, m_currMouse;
    };

} // namespace Tektite
