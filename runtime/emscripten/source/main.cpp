#include <core/engine/engine.hpp>

#include <emscripten.h>
#include <emscripten/dom_pk_codes.h>
#include <emscripten/html5.h>

struct EmscriptenState
{
    EMSCRIPTEN_WEBGL_CONTEXT_HANDLE context;

    double width, height;
    Tektite::Engine *engine;
} static state;

Tektite::Keyboard::Key translateKey(DOM_PK_CODE_TYPE keyCode);

EM_BOOL windowResizeEvent(int eventType, const EmscriptenUiEvent *uiEvent, void *userData)
{
    emscripten_get_element_css_size("#canvas", &state.width, &state.height);
    state.engine->getGraphics()->resize(state.width, state.height);
    return false;
}

EM_BOOL keyDownEvent(int eventType, const EmscriptenKeyboardEvent *keyEvent, void *userData)
{
    Tektite::Keyboard::Key key = translateKey(emscripten_compute_dom_pk_code(keyEvent->code));
    state.engine->getInput()->onPress(key);
    return true;
}

EM_BOOL keyUpEvent(int eventType, const EmscriptenKeyboardEvent *keyEvent, void *userData)
{
    Tektite::Keyboard::Key key = translateKey(emscripten_compute_dom_pk_code(keyEvent->code));
    state.engine->getInput()->onRelease(key);
    return true;
}

EM_BOOL mouseDownEvent(int eventType, const EmscriptenMouseEvent *mouseEvent, void *userData)
{
    switch (mouseEvent->button) {
    case 0:
        state.engine->getInput()->onPress(Tektite::Mouse::Left);
        break;
    case 1:
        state.engine->getInput()->onPress(Tektite::Mouse::Middle);
        break;
    case 2:
        state.engine->getInput()->onPress(Tektite::Mouse::Right);
        break;
    default:
        state.engine->getInput()->onPress(Tektite::Mouse::Unknown);
        break;
    }
    return false;
}

EM_BOOL mouseUpEvent(int eventType, const EmscriptenMouseEvent *mouseEvent, void *userData)
{
    switch (mouseEvent->button) {
    case 0:
        state.engine->getInput()->onRelease(Tektite::Mouse::Left);
        break;
    case 1:
        state.engine->getInput()->onRelease(Tektite::Mouse::Middle);
        break;
    case 2:
        state.engine->getInput()->onRelease(Tektite::Mouse::Right);
        break;
    default:
        state.engine->getInput()->onRelease(Tektite::Mouse::Unknown);
        break;
    }
    return false;
}

EM_BOOL mouseMoveEvent(int eventType, const EmscriptenMouseEvent *mouseEvent, void *userData)
{
    state.engine->getInput()->onMove(mouseEvent->clientX, mouseEvent->clientY);
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

    state.engine = new Tektite::Engine();
    state.engine->getGraphics()->resize(state.width, state.height);

    emscripten_set_keydown_callback_on_thread("#canvas", nullptr, false, keyDownEvent, 0);
    emscripten_set_keyup_callback_on_thread("#canvas", nullptr, false, keyUpEvent, 0);
    emscripten_set_mousedown_callback_on_thread("#canvas", nullptr, false, mouseDownEvent, 0);
    emscripten_set_mouseup_callback_on_thread("#canvas", nullptr, false, mouseUpEvent, 0);
    emscripten_set_mousemove_callback_on_thread("#canvas", nullptr, false, mouseMoveEvent, 0);
    emscripten_set_resize_callback_on_thread(EMSCRIPTEN_EVENT_TARGET_WINDOW, nullptr, false, windowResizeEvent, 0);

    auto loop = []() {
        state.engine->run();
        emscripten_webgl_commit_frame();
    };

    emscripten_set_main_loop(loop, 0, true);
    emscripten_webgl_destroy_context(state.context);

    delete state.engine;

    return 0;
}

Tektite::Keyboard::Key translateKey(DOM_PK_CODE_TYPE keyCode)
{
    switch (keyCode) {
    case DOM_PK_ARROW_UP:
        return Tektite::Keyboard::Up;
    case DOM_PK_ARROW_DOWN:
        return Tektite::Keyboard::Down;
    case DOM_PK_ARROW_LEFT:
        return Tektite::Keyboard::Left;
    case DOM_PK_ARROW_RIGHT:
        return Tektite::Keyboard::Right;
    case DOM_PK_ESCAPE:
        return Tektite::Keyboard::Escape;
    case DOM_PK_ENTER:
        return Tektite::Keyboard::Enter;
    case DOM_PK_BACKSPACE:
        return Tektite::Keyboard::Backspace;
    case DOM_PK_HOME:
        return Tektite::Keyboard::Home;
    case DOM_PK_INSERT:
        return Tektite::Keyboard::Insert;
    case DOM_PK_PAGE_UP:
        return Tektite::Keyboard::PageUp;
    case DOM_PK_PAGE_DOWN:
        return Tektite::Keyboard::PageDown;
    case DOM_PK_DELETE:
        return Tektite::Keyboard::Delete;
    case DOM_PK_END:
        return Tektite::Keyboard::End;
    case DOM_PK_PRINT_SCREEN:
        return Tektite::Keyboard::PrintScreen;
    case DOM_PK_PAUSE:
        return Tektite::Keyboard::Pause;
    case DOM_PK_SCROLL_LOCK:
        return Tektite::Keyboard::ScrollLock;
    case DOM_PK_NUM_LOCK:
        return Tektite::Keyboard::NumLock;
    case DOM_PK_CAPS_LOCK:
        return Tektite::Keyboard::CapsLock;
    case DOM_PK_TAB:
        return Tektite::Keyboard::Tab;
    case DOM_PK_BACKSLASH:
        return Tektite::Keyboard::Backslash;
    case DOM_PK_SPACE:
        return Tektite::Keyboard::Space;
    case DOM_PK_SHIFT_LEFT:
        return Tektite::Keyboard::LeftShift;
    case DOM_PK_SHIFT_RIGHT:
        return Tektite::Keyboard::RightShift;
    case DOM_PK_CONTROL_LEFT:
        return Tektite::Keyboard::LeftCtrl;
    case DOM_PK_CONTROL_RIGHT:
        return Tektite::Keyboard::RightCtrl;
    case DOM_PK_ALT_LEFT:
        return Tektite::Keyboard::LeftAlt;
    case DOM_PK_ALT_RIGHT:
        return Tektite::Keyboard::RightAlt;
    case DOM_PK_NUMPAD_0:
        return Tektite::Keyboard::Numpad0;
    case DOM_PK_NUMPAD_1:
        return Tektite::Keyboard::Numpad1;
    case DOM_PK_NUMPAD_2:
        return Tektite::Keyboard::Numpad2;
    case DOM_PK_NUMPAD_3:
        return Tektite::Keyboard::Numpad3;
    case DOM_PK_NUMPAD_4:
        return Tektite::Keyboard::Numpad4;
    case DOM_PK_NUMPAD_5:
        return Tektite::Keyboard::Numpad5;
    case DOM_PK_NUMPAD_6:
        return Tektite::Keyboard::Numpad6;
    case DOM_PK_NUMPAD_7:
        return Tektite::Keyboard::Numpad7;
    case DOM_PK_NUMPAD_8:
        return Tektite::Keyboard::Numpad8;
    case DOM_PK_NUMPAD_9:
        return Tektite::Keyboard::Numpad9;
    case DOM_PK_A:
        return Tektite::Keyboard::A;
    case DOM_PK_B:
        return Tektite::Keyboard::B;
    case DOM_PK_C:
        return Tektite::Keyboard::C;
    case DOM_PK_D:
        return Tektite::Keyboard::D;
    case DOM_PK_E:
        return Tektite::Keyboard::E;
    case DOM_PK_F:
        return Tektite::Keyboard::F;
    case DOM_PK_G:
        return Tektite::Keyboard::G;
    case DOM_PK_H:
        return Tektite::Keyboard::H;
    case DOM_PK_I:
        return Tektite::Keyboard::I;
    case DOM_PK_J:
        return Tektite::Keyboard::J;
    case DOM_PK_K:
        return Tektite::Keyboard::K;
    case DOM_PK_L:
        return Tektite::Keyboard::L;
    case DOM_PK_M:
        return Tektite::Keyboard::M;
    case DOM_PK_N:
        return Tektite::Keyboard::N;
    case DOM_PK_O:
        return Tektite::Keyboard::O;
    case DOM_PK_P:
        return Tektite::Keyboard::P;
    case DOM_PK_Q:
        return Tektite::Keyboard::Q;
    case DOM_PK_R:
        return Tektite::Keyboard::R;
    case DOM_PK_S:
        return Tektite::Keyboard::S;
    case DOM_PK_T:
        return Tektite::Keyboard::T;
    case DOM_PK_U:
        return Tektite::Keyboard::U;
    case DOM_PK_V:
        return Tektite::Keyboard::V;
    case DOM_PK_W:
        return Tektite::Keyboard::W;
    case DOM_PK_X:
        return Tektite::Keyboard::X;
    case DOM_PK_Y:
        return Tektite::Keyboard::Y;
    case DOM_PK_Z:
        return Tektite::Keyboard::Z;
    case DOM_PK_0:
        return Tektite::Keyboard::Num0;
    case DOM_PK_1:
        return Tektite::Keyboard::Num1;
    case DOM_PK_2:
        return Tektite::Keyboard::Num2;
    case DOM_PK_3:
        return Tektite::Keyboard::Num3;
    case DOM_PK_4:
        return Tektite::Keyboard::Num4;
    case DOM_PK_5:
        return Tektite::Keyboard::Num5;
    case DOM_PK_6:
        return Tektite::Keyboard::Num6;
    case DOM_PK_7:
        return Tektite::Keyboard::Num7;
    case DOM_PK_8:
        return Tektite::Keyboard::Num8;
    case DOM_PK_9:
        return Tektite::Keyboard::Num9;
    case DOM_PK_F1:
        return Tektite::Keyboard::F1;
    case DOM_PK_F2:
        return Tektite::Keyboard::F2;
    case DOM_PK_F3:
        return Tektite::Keyboard::F3;
    case DOM_PK_F4:
        return Tektite::Keyboard::F4;
    case DOM_PK_F5:
        return Tektite::Keyboard::F5;
    case DOM_PK_F6:
        return Tektite::Keyboard::F6;
    case DOM_PK_F7:
        return Tektite::Keyboard::F7;
    case DOM_PK_F8:
        return Tektite::Keyboard::F8;
    case DOM_PK_F9:
        return Tektite::Keyboard::F9;
    case DOM_PK_F10:
        return Tektite::Keyboard::F10;
    case DOM_PK_F11:
        return Tektite::Keyboard::F11;
    case DOM_PK_F12:
        return Tektite::Keyboard::F12;
    }

    return Tektite::Keyboard::Unknown;
}
