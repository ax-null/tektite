#pragma once

#include <stdint.h>

namespace paper
{

    namespace keyboard
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
            bool keys[static_cast<uint32_t>(keyboard::Unknown)];
        };

    } // namespace keyboard

    namespace mouse
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
            bool buttons[static_cast<uint32_t>(mouse::Unknown)];
        };

    } // namespace mouse

    class InputSystem
    {
      public:
        InputSystem() = default;
        ~InputSystem() = default;

        void update();

        void on_move(float x, float y);

        void on_press(keyboard::Key key);
        void on_press(mouse::Button button);

        void on_release(keyboard::Key key);
        void on_release(mouse::Button button);

        float get_x() const;
        float get_y() const;

        float get_delta_x() const;
        float get_delta_y() const;

        bool is_down(keyboard::Key key) const;
        bool is_down(mouse::Button button) const;

        bool is_pressed(keyboard::Key key) const;
        bool is_pressed(mouse::Button button) const;

        bool is_released(keyboard::Key key) const;
        bool is_released(mouse::Button button) const;

        bool is_held(keyboard::Key key) const;
        bool is_held(mouse::Button button) const;

      private:
        keyboard::State m_keys, m_prev_keys, m_curr_keys;
        mouse::State m_mouse, m_prev_mouse, m_curr_mouse;
    };

} // namespace paper
