#include <engine/engine.hpp>

#include <emscripten.h>
#include <emscripten/dom_pk_codes.h>
#include <emscripten/html5.h>

struct EmscriptenState
{
    EMSCRIPTEN_WEBGL_CONTEXT_HANDLE context;

    double width, height;
    tektite::Engine *engine;
} static state;

tektite::keyboard::Key translate_key(DOM_PK_CODE_TYPE keyCode);

EM_BOOL window_resize_event(int eventType, const EmscriptenUiEvent *uiEvent, void *userData)
{
    emscripten_get_element_css_size("#canvas", &state.width, &state.height);
    state.engine->get_graphics()->resize(state.width, state.height);
    return false;
}

EM_BOOL key_down_event(int eventType, const EmscriptenKeyboardEvent *keyEvent, void *userData)
{
    tektite::keyboard::Key key = translate_key(emscripten_compute_dom_pk_code(keyEvent->code));
    state.engine->get_input_system()->on_press(key);
    return true;
}

EM_BOOL key_up_event(int eventType, const EmscriptenKeyboardEvent *keyEvent, void *userData)
{
    tektite::keyboard::Key key = translate_key(emscripten_compute_dom_pk_code(keyEvent->code));
    state.engine->get_input_system()->on_release(key);
    return true;
}

EM_BOOL mouse_down_event(int eventType, const EmscriptenMouseEvent *mouseEvent, void *userData)
{
    switch (mouseEvent->button) {
    case 0:
        state.engine->get_input_system()->on_press(tektite::mouse::Left);
        break;
    case 1:
        state.engine->get_input_system()->on_press(tektite::mouse::Middle);
        break;
    case 2:
        state.engine->get_input_system()->on_press(tektite::mouse::Right);
        break;
    default:
        state.engine->get_input_system()->on_press(tektite::mouse::Unknown);
        break;
    }
    return false;
}

EM_BOOL mouse_up_event(int eventType, const EmscriptenMouseEvent *mouseEvent, void *userData)
{
    switch (mouseEvent->button) {
    case 0:
        state.engine->get_input_system()->on_release(tektite::mouse::Left);
        break;
    case 1:
        state.engine->get_input_system()->on_release(tektite::mouse::Middle);
        break;
    case 2:
        state.engine->get_input_system()->on_release(tektite::mouse::Right);
        break;
    default:
        state.engine->get_input_system()->on_release(tektite::mouse::Unknown);
        break;
    }
    return false;
}

EM_BOOL mouse_move_event(int eventType, const EmscriptenMouseEvent *mouseEvent, void *userData)
{
    state.engine->get_input_system()->on_move(mouseEvent->clientX, mouseEvent->clientY);
    return false;
}

int main(int argc, char **argv)
{
    state = EmscriptenState();

    emscripten_set_window_title("Tektite");
    emscripten_get_element_css_size("#canvas", &state.width, &state.height);

    EmscriptenWebGLContextAttributes attributes = {
        true, true, true, true, true, true,
        EM_WEBGL_POWER_PREFERENCE_DEFAULT,
        false, 2, 0, true, false,
        EMSCRIPTEN_WEBGL_CONTEXT_PROXY_DISALLOW, false
    };

    state.context = emscripten_webgl_create_context("#canvas", &attributes);
    emscripten_webgl_make_context_current(state.context);

    state.engine = new tektite::Engine();
    state.engine->get_graphics()->resize(state.width, state.height);

    emscripten_set_resize_callback_on_thread(EMSCRIPTEN_EVENT_TARGET_WINDOW, nullptr, false, window_resize_event, 0);
    emscripten_set_keydown_callback_on_thread("#canvas", nullptr, false, key_down_event, 0);
    emscripten_set_keyup_callback_on_thread("#canvas", nullptr, false, key_up_event, 0);
    emscripten_set_mousedown_callback_on_thread("#canvas", nullptr, false, mouse_down_event, 0);
    emscripten_set_mouseup_callback_on_thread("#canvas", nullptr, false, mouse_up_event, 0);
    emscripten_set_mousemove_callback_on_thread("#canvas", nullptr, false, mouse_move_event, 0);

    auto loop = []() {
        state.engine->run();
        emscripten_webgl_commit_frame();
    };

    emscripten_set_main_loop(loop, 0, true);
    emscripten_webgl_destroy_context(state.context);

    delete state.engine;

    return 0;
}

tektite::keyboard::Key translate_key(DOM_PK_CODE_TYPE keyCode)
{
    switch (keyCode) {
    case DOM_PK_ARROW_UP:
        return tektite::keyboard::Up;
    case DOM_PK_ARROW_DOWN:
        return tektite::keyboard::Down;
    case DOM_PK_ARROW_LEFT:
        return tektite::keyboard::Left;
    case DOM_PK_ARROW_RIGHT:
        return tektite::keyboard::Right;
    case DOM_PK_ESCAPE:
        return tektite::keyboard::Escape;
    case DOM_PK_ENTER:
        return tektite::keyboard::Enter;
    case DOM_PK_BACKSPACE:
        return tektite::keyboard::Backspace;
    case DOM_PK_HOME:
        return tektite::keyboard::Home;
    case DOM_PK_INSERT:
        return tektite::keyboard::Insert;
    case DOM_PK_PAGE_UP:
        return tektite::keyboard::PageUp;
    case DOM_PK_PAGE_DOWN:
        return tektite::keyboard::PageDown;
    case DOM_PK_DELETE:
        return tektite::keyboard::Delete;
    case DOM_PK_END:
        return tektite::keyboard::End;
    case DOM_PK_PRINT_SCREEN:
        return tektite::keyboard::PrintScreen;
    case DOM_PK_PAUSE:
        return tektite::keyboard::Pause;
    case DOM_PK_SCROLL_LOCK:
        return tektite::keyboard::ScrollLock;
    case DOM_PK_NUM_LOCK:
        return tektite::keyboard::NumLock;
    case DOM_PK_CAPS_LOCK:
        return tektite::keyboard::CapsLock;
    case DOM_PK_TAB:
        return tektite::keyboard::Tab;
    case DOM_PK_BACKSLASH:
        return tektite::keyboard::Backslash;
    case DOM_PK_SPACE:
        return tektite::keyboard::Space;
    case DOM_PK_SHIFT_LEFT:
        return tektite::keyboard::LeftShift;
    case DOM_PK_SHIFT_RIGHT:
        return tektite::keyboard::RightShift;
    case DOM_PK_CONTROL_LEFT:
        return tektite::keyboard::LeftCtrl;
    case DOM_PK_CONTROL_RIGHT:
        return tektite::keyboard::RightCtrl;
    case DOM_PK_ALT_LEFT:
        return tektite::keyboard::LeftAlt;
    case DOM_PK_ALT_RIGHT:
        return tektite::keyboard::RightAlt;
    case DOM_PK_NUMPAD_0:
        return tektite::keyboard::Numpad0;
    case DOM_PK_NUMPAD_1:
        return tektite::keyboard::Numpad1;
    case DOM_PK_NUMPAD_2:
        return tektite::keyboard::Numpad2;
    case DOM_PK_NUMPAD_3:
        return tektite::keyboard::Numpad3;
    case DOM_PK_NUMPAD_4:
        return tektite::keyboard::Numpad4;
    case DOM_PK_NUMPAD_5:
        return tektite::keyboard::Numpad5;
    case DOM_PK_NUMPAD_6:
        return tektite::keyboard::Numpad6;
    case DOM_PK_NUMPAD_7:
        return tektite::keyboard::Numpad7;
    case DOM_PK_NUMPAD_8:
        return tektite::keyboard::Numpad8;
    case DOM_PK_NUMPAD_9:
        return tektite::keyboard::Numpad9;
    case DOM_PK_A:
        return tektite::keyboard::A;
    case DOM_PK_B:
        return tektite::keyboard::B;
    case DOM_PK_C:
        return tektite::keyboard::C;
    case DOM_PK_D:
        return tektite::keyboard::D;
    case DOM_PK_E:
        return tektite::keyboard::E;
    case DOM_PK_F:
        return tektite::keyboard::F;
    case DOM_PK_G:
        return tektite::keyboard::G;
    case DOM_PK_H:
        return tektite::keyboard::H;
    case DOM_PK_I:
        return tektite::keyboard::I;
    case DOM_PK_J:
        return tektite::keyboard::J;
    case DOM_PK_K:
        return tektite::keyboard::K;
    case DOM_PK_L:
        return tektite::keyboard::L;
    case DOM_PK_M:
        return tektite::keyboard::M;
    case DOM_PK_N:
        return tektite::keyboard::N;
    case DOM_PK_O:
        return tektite::keyboard::O;
    case DOM_PK_P:
        return tektite::keyboard::P;
    case DOM_PK_Q:
        return tektite::keyboard::Q;
    case DOM_PK_R:
        return tektite::keyboard::R;
    case DOM_PK_S:
        return tektite::keyboard::S;
    case DOM_PK_T:
        return tektite::keyboard::T;
    case DOM_PK_U:
        return tektite::keyboard::U;
    case DOM_PK_V:
        return tektite::keyboard::V;
    case DOM_PK_W:
        return tektite::keyboard::W;
    case DOM_PK_X:
        return tektite::keyboard::X;
    case DOM_PK_Y:
        return tektite::keyboard::Y;
    case DOM_PK_Z:
        return tektite::keyboard::Z;
    case DOM_PK_0:
        return tektite::keyboard::Num0;
    case DOM_PK_1:
        return tektite::keyboard::Num1;
    case DOM_PK_2:
        return tektite::keyboard::Num2;
    case DOM_PK_3:
        return tektite::keyboard::Num3;
    case DOM_PK_4:
        return tektite::keyboard::Num4;
    case DOM_PK_5:
        return tektite::keyboard::Num5;
    case DOM_PK_6:
        return tektite::keyboard::Num6;
    case DOM_PK_7:
        return tektite::keyboard::Num7;
    case DOM_PK_8:
        return tektite::keyboard::Num8;
    case DOM_PK_9:
        return tektite::keyboard::Num9;
    case DOM_PK_F1:
        return tektite::keyboard::F1;
    case DOM_PK_F2:
        return tektite::keyboard::F2;
    case DOM_PK_F3:
        return tektite::keyboard::F3;
    case DOM_PK_F4:
        return tektite::keyboard::F4;
    case DOM_PK_F5:
        return tektite::keyboard::F5;
    case DOM_PK_F6:
        return tektite::keyboard::F6;
    case DOM_PK_F7:
        return tektite::keyboard::F7;
    case DOM_PK_F8:
        return tektite::keyboard::F8;
    case DOM_PK_F9:
        return tektite::keyboard::F9;
    case DOM_PK_F10:
        return tektite::keyboard::F10;
    case DOM_PK_F11:
        return tektite::keyboard::F11;
    case DOM_PK_F12:
        return tektite::keyboard::F12;
    }

    return tektite::keyboard::Unknown;
}
