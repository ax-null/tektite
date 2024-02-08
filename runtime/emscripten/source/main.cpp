#include <engine/engine.hpp>

#include <emscripten.h>
#include <emscripten/dom_pk_codes.h>
#include <emscripten/html5.h>

struct EmscriptenState
{
    EMSCRIPTEN_WEBGL_CONTEXT_HANDLE context;

    double width, height;
    paper::Engine *engine;
} static state;

paper::keyboard::Key translate_key(DOM_PK_CODE_TYPE keyCode);

EM_BOOL window_resize_event(int eventType, const EmscriptenUiEvent *uiEvent, void *userData)
{
    emscripten_get_element_css_size("#canvas", &state.width, &state.height);
    state.engine->get_graphics()->resize(state.width, state.height);
    return false;
}

EM_BOOL key_down_event(int eventType, const EmscriptenKeyboardEvent *keyEvent, void *userData)
{
    paper::keyboard::Key key = translate_key(emscripten_compute_dom_pk_code(keyEvent->code));
    state.engine->get_input_system()->on_press(key);
    return true;
}

EM_BOOL key_up_event(int eventType, const EmscriptenKeyboardEvent *keyEvent, void *userData)
{
    paper::keyboard::Key key = translate_key(emscripten_compute_dom_pk_code(keyEvent->code));
    state.engine->get_input_system()->on_release(key);
    return true;
}

EM_BOOL mouse_down_event(int eventType, const EmscriptenMouseEvent *mouseEvent, void *userData)
{
    switch (mouseEvent->button) {
    case 0:
        state.engine->get_input_system()->on_press(paper::mouse::Left);
        break;
    case 1:
        state.engine->get_input_system()->on_press(paper::mouse::Middle);
        break;
    case 2:
        state.engine->get_input_system()->on_press(paper::mouse::Right);
        break;
    default:
        state.engine->get_input_system()->on_press(paper::mouse::Unknown);
        break;
    }
    return false;
}

EM_BOOL mouse_up_event(int eventType, const EmscriptenMouseEvent *mouseEvent, void *userData)
{
    switch (mouseEvent->button) {
    case 0:
        state.engine->get_input_system()->on_release(paper::mouse::Left);
        break;
    case 1:
        state.engine->get_input_system()->on_release(paper::mouse::Middle);
        break;
    case 2:
        state.engine->get_input_system()->on_release(paper::mouse::Right);
        break;
    default:
        state.engine->get_input_system()->on_release(paper::mouse::Unknown);
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

    emscripten_set_window_title("Paper");
    emscripten_get_element_css_size("#canvas", &state.width, &state.height);

    EmscriptenWebGLContextAttributes attributes = {
        true, true, true, true, true, true,
        EM_WEBGL_POWER_PREFERENCE_DEFAULT,
        false, 2, 0, true, false,
        EMSCRIPTEN_WEBGL_CONTEXT_PROXY_DISALLOW, false
    };

    state.context = emscripten_webgl_create_context("#canvas", &attributes);
    emscripten_webgl_make_context_current(state.context);

    state.engine = new paper::Engine();
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

paper::keyboard::Key translate_key(DOM_PK_CODE_TYPE keyCode)
{
    switch (keyCode) {
    case DOM_PK_ARROW_UP:
        return paper::keyboard::Up;
    case DOM_PK_ARROW_DOWN:
        return paper::keyboard::Down;
    case DOM_PK_ARROW_LEFT:
        return paper::keyboard::Left;
    case DOM_PK_ARROW_RIGHT:
        return paper::keyboard::Right;
    case DOM_PK_ESCAPE:
        return paper::keyboard::Escape;
    case DOM_PK_ENTER:
        return paper::keyboard::Enter;
    case DOM_PK_BACKSPACE:
        return paper::keyboard::Backspace;
    case DOM_PK_HOME:
        return paper::keyboard::Home;
    case DOM_PK_INSERT:
        return paper::keyboard::Insert;
    case DOM_PK_PAGE_UP:
        return paper::keyboard::PageUp;
    case DOM_PK_PAGE_DOWN:
        return paper::keyboard::PageDown;
    case DOM_PK_DELETE:
        return paper::keyboard::Delete;
    case DOM_PK_END:
        return paper::keyboard::End;
    case DOM_PK_PRINT_SCREEN:
        return paper::keyboard::PrintScreen;
    case DOM_PK_PAUSE:
        return paper::keyboard::Pause;
    case DOM_PK_SCROLL_LOCK:
        return paper::keyboard::ScrollLock;
    case DOM_PK_NUM_LOCK:
        return paper::keyboard::NumLock;
    case DOM_PK_CAPS_LOCK:
        return paper::keyboard::CapsLock;
    case DOM_PK_TAB:
        return paper::keyboard::Tab;
    case DOM_PK_BACKSLASH:
        return paper::keyboard::Backslash;
    case DOM_PK_SPACE:
        return paper::keyboard::Space;
    case DOM_PK_SHIFT_LEFT:
        return paper::keyboard::LeftShift;
    case DOM_PK_SHIFT_RIGHT:
        return paper::keyboard::RightShift;
    case DOM_PK_CONTROL_LEFT:
        return paper::keyboard::LeftCtrl;
    case DOM_PK_CONTROL_RIGHT:
        return paper::keyboard::RightCtrl;
    case DOM_PK_ALT_LEFT:
        return paper::keyboard::LeftAlt;
    case DOM_PK_ALT_RIGHT:
        return paper::keyboard::RightAlt;
    case DOM_PK_NUMPAD_0:
        return paper::keyboard::Numpad0;
    case DOM_PK_NUMPAD_1:
        return paper::keyboard::Numpad1;
    case DOM_PK_NUMPAD_2:
        return paper::keyboard::Numpad2;
    case DOM_PK_NUMPAD_3:
        return paper::keyboard::Numpad3;
    case DOM_PK_NUMPAD_4:
        return paper::keyboard::Numpad4;
    case DOM_PK_NUMPAD_5:
        return paper::keyboard::Numpad5;
    case DOM_PK_NUMPAD_6:
        return paper::keyboard::Numpad6;
    case DOM_PK_NUMPAD_7:
        return paper::keyboard::Numpad7;
    case DOM_PK_NUMPAD_8:
        return paper::keyboard::Numpad8;
    case DOM_PK_NUMPAD_9:
        return paper::keyboard::Numpad9;
    case DOM_PK_A:
        return paper::keyboard::A;
    case DOM_PK_B:
        return paper::keyboard::B;
    case DOM_PK_C:
        return paper::keyboard::C;
    case DOM_PK_D:
        return paper::keyboard::D;
    case DOM_PK_E:
        return paper::keyboard::E;
    case DOM_PK_F:
        return paper::keyboard::F;
    case DOM_PK_G:
        return paper::keyboard::G;
    case DOM_PK_H:
        return paper::keyboard::H;
    case DOM_PK_I:
        return paper::keyboard::I;
    case DOM_PK_J:
        return paper::keyboard::J;
    case DOM_PK_K:
        return paper::keyboard::K;
    case DOM_PK_L:
        return paper::keyboard::L;
    case DOM_PK_M:
        return paper::keyboard::M;
    case DOM_PK_N:
        return paper::keyboard::N;
    case DOM_PK_O:
        return paper::keyboard::O;
    case DOM_PK_P:
        return paper::keyboard::P;
    case DOM_PK_Q:
        return paper::keyboard::Q;
    case DOM_PK_R:
        return paper::keyboard::R;
    case DOM_PK_S:
        return paper::keyboard::S;
    case DOM_PK_T:
        return paper::keyboard::T;
    case DOM_PK_U:
        return paper::keyboard::U;
    case DOM_PK_V:
        return paper::keyboard::V;
    case DOM_PK_W:
        return paper::keyboard::W;
    case DOM_PK_X:
        return paper::keyboard::X;
    case DOM_PK_Y:
        return paper::keyboard::Y;
    case DOM_PK_Z:
        return paper::keyboard::Z;
    case DOM_PK_0:
        return paper::keyboard::Num0;
    case DOM_PK_1:
        return paper::keyboard::Num1;
    case DOM_PK_2:
        return paper::keyboard::Num2;
    case DOM_PK_3:
        return paper::keyboard::Num3;
    case DOM_PK_4:
        return paper::keyboard::Num4;
    case DOM_PK_5:
        return paper::keyboard::Num5;
    case DOM_PK_6:
        return paper::keyboard::Num6;
    case DOM_PK_7:
        return paper::keyboard::Num7;
    case DOM_PK_8:
        return paper::keyboard::Num8;
    case DOM_PK_9:
        return paper::keyboard::Num9;
    case DOM_PK_F1:
        return paper::keyboard::F1;
    case DOM_PK_F2:
        return paper::keyboard::F2;
    case DOM_PK_F3:
        return paper::keyboard::F3;
    case DOM_PK_F4:
        return paper::keyboard::F4;
    case DOM_PK_F5:
        return paper::keyboard::F5;
    case DOM_PK_F6:
        return paper::keyboard::F6;
    case DOM_PK_F7:
        return paper::keyboard::F7;
    case DOM_PK_F8:
        return paper::keyboard::F8;
    case DOM_PK_F9:
        return paper::keyboard::F9;
    case DOM_PK_F10:
        return paper::keyboard::F10;
    case DOM_PK_F11:
        return paper::keyboard::F11;
    case DOM_PK_F12:
        return paper::keyboard::F12;
    }

    return paper::keyboard::Unknown;
}
