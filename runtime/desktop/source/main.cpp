#include <engine/engine.hpp>

#include <stdio.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <glad/gl.h>

struct DesktopState
{
    GLFWwindow *window;
    int32_t width, height;
    tektite::Engine *engine;
} static state;

tektite::keyboard::Key translate_key(int keyCode);

void error_callback(int error, const char *description)
{
    fprintf(stderr, "Error: %s\n", description);
}

void window_size_callback(GLFWwindow *window, int width, int height)
{
    if (state.engine != nullptr)
        state.engine->get_graphics()->resize(width, height);
}

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS)
        state.engine->get_input_system()->on_press(translate_key(key));
    else if (action == GLFW_RELEASE)
        state.engine->get_input_system()->on_release(translate_key(key));
}

void mouse_button_callback(GLFWwindow *window, int button, int action, int mods)
{
    tektite::mouse::Button mouse_button;

    switch (button) {
    case GLFW_MOUSE_BUTTON_LEFT:
        mouse_button = tektite::mouse::Left;
        break;
    case GLFW_MOUSE_BUTTON_MIDDLE:
        mouse_button = tektite::mouse::Middle;
        break;
    case GLFW_MOUSE_BUTTON_RIGHT:
        mouse_button = tektite::mouse::Right;
        break;
    default:
        mouse_button = tektite::mouse::Unknown;
    }

    if (action == GLFW_PRESS)
        state.engine->get_input_system()->on_press(mouse_button);
    else if (action == GLFW_RELEASE)
        state.engine->get_input_system()->on_release(mouse_button);
}

void cursor_position_callback(GLFWwindow *window, double xpos, double ypos)
{
    state.engine->get_input_system()->on_move(xpos, ypos);
}

int main(int argc, char **argv)
{
    state = DesktopState();
    state.width = 800;
    state.height = 600;

    if (!glfwInit()) {
        printf("Couldn't initialize glfw.\n");
        return -1;
    }

    glfwSetErrorCallback(error_callback);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    state.window = glfwCreateWindow(state.width, state.height, "Tektite", nullptr, nullptr);

    if (!state.window) {
        printf("Couldn't create window.\n");
        return -1;
    }

    const GLFWvidmode *video_mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    glfwSetWindowPos(state.window,
                     (video_mode->width - state.width) / 2,
                     (video_mode->height - state.height) / 2);

    glfwSetWindowSizeCallback(state.window, window_size_callback);

    glfwSetKeyCallback(state.window, key_callback);
    glfwSetMouseButtonCallback(state.window, mouse_button_callback);
    glfwSetCursorPosCallback(state.window, cursor_position_callback);

    glfwMakeContextCurrent(state.window);
    glfwSwapInterval(1);

    gladLoadGL(glfwGetProcAddress);

    state.engine = new tektite::Engine();
    state.engine->get_graphics()->resize(800, 600);

    while (state.engine->is_running() && !glfwWindowShouldClose(state.window)) {
        glfwPollEvents();

        state.engine->run();
        glfwSwapBuffers(state.window);
    }

    delete state.engine;

    glfwDestroyWindow(state.window);
    glfwTerminate();

    return 0;
}

tektite::keyboard::Key translate_key(int keyCode)
{
    switch (keyCode) {
    case GLFW_KEY_UP:
        return tektite::keyboard::Up;
    case GLFW_KEY_DOWN:
        return tektite::keyboard::Down;
    case GLFW_KEY_LEFT:
        return tektite::keyboard::Left;
    case GLFW_KEY_RIGHT:
        return tektite::keyboard::Right;
    case GLFW_KEY_ESCAPE:
        return tektite::keyboard::Escape;
    case GLFW_KEY_ENTER:
        return tektite::keyboard::Enter;
    case GLFW_KEY_BACKSPACE:
        return tektite::keyboard::Backspace;
    case GLFW_KEY_HOME:
        return tektite::keyboard::Home;
    case GLFW_KEY_INSERT:
        return tektite::keyboard::Insert;
    case GLFW_KEY_PAGE_UP:
        return tektite::keyboard::PageUp;
    case GLFW_KEY_PAGE_DOWN:
        return tektite::keyboard::PageDown;
    case GLFW_KEY_DELETE:
        return tektite::keyboard::Delete;
    case GLFW_KEY_END:
        return tektite::keyboard::End;
    case GLFW_KEY_PRINT_SCREEN:
        return tektite::keyboard::PrintScreen;
    case GLFW_KEY_PAUSE:
        return tektite::keyboard::Pause;
    case GLFW_KEY_SCROLL_LOCK:
        return tektite::keyboard::ScrollLock;
    case GLFW_KEY_NUM_LOCK:
        return tektite::keyboard::NumLock;
    case GLFW_KEY_CAPS_LOCK:
        return tektite::keyboard::CapsLock;
    case GLFW_KEY_TAB:
        return tektite::keyboard::Tab;
    case GLFW_KEY_BACKSLASH:
        return tektite::keyboard::Backslash;
    case GLFW_KEY_SPACE:
        return tektite::keyboard::Space;
    case GLFW_KEY_LEFT_SHIFT:
        return tektite::keyboard::LeftShift;
    case GLFW_KEY_RIGHT_SHIFT:
        return tektite::keyboard::RightShift;
    case GLFW_KEY_LEFT_CONTROL:
        return tektite::keyboard::LeftCtrl;
    case GLFW_KEY_RIGHT_CONTROL:
        return tektite::keyboard::RightCtrl;
    case GLFW_KEY_LEFT_ALT:
        return tektite::keyboard::LeftAlt;
    case GLFW_KEY_RIGHT_ALT:
        return tektite::keyboard::RightAlt;
    case GLFW_KEY_KP_0:
        return tektite::keyboard::Numpad0;
    case GLFW_KEY_KP_1:
        return tektite::keyboard::Numpad1;
    case GLFW_KEY_KP_2:
        return tektite::keyboard::Numpad2;
    case GLFW_KEY_KP_3:
        return tektite::keyboard::Numpad3;
    case GLFW_KEY_KP_4:
        return tektite::keyboard::Numpad4;
    case GLFW_KEY_KP_5:
        return tektite::keyboard::Numpad5;
    case GLFW_KEY_KP_6:
        return tektite::keyboard::Numpad6;
    case GLFW_KEY_KP_7:
        return tektite::keyboard::Numpad7;
    case GLFW_KEY_KP_8:
        return tektite::keyboard::Numpad8;
    case GLFW_KEY_KP_9:
        return tektite::keyboard::Numpad9;
    case GLFW_KEY_A:
        return tektite::keyboard::A;
    case GLFW_KEY_B:
        return tektite::keyboard::B;
    case GLFW_KEY_C:
        return tektite::keyboard::C;
    case GLFW_KEY_D:
        return tektite::keyboard::D;
    case GLFW_KEY_E:
        return tektite::keyboard::E;
    case GLFW_KEY_F:
        return tektite::keyboard::F;
    case GLFW_KEY_G:
        return tektite::keyboard::G;
    case GLFW_KEY_H:
        return tektite::keyboard::H;
    case GLFW_KEY_I:
        return tektite::keyboard::I;
    case GLFW_KEY_J:
        return tektite::keyboard::J;
    case GLFW_KEY_K:
        return tektite::keyboard::K;
    case GLFW_KEY_L:
        return tektite::keyboard::L;
    case GLFW_KEY_M:
        return tektite::keyboard::M;
    case GLFW_KEY_N:
        return tektite::keyboard::N;
    case GLFW_KEY_O:
        return tektite::keyboard::O;
    case GLFW_KEY_P:
        return tektite::keyboard::P;
    case GLFW_KEY_Q:
        return tektite::keyboard::Q;
    case GLFW_KEY_R:
        return tektite::keyboard::R;
    case GLFW_KEY_S:
        return tektite::keyboard::S;
    case GLFW_KEY_T:
        return tektite::keyboard::T;
    case GLFW_KEY_U:
        return tektite::keyboard::U;
    case GLFW_KEY_V:
        return tektite::keyboard::V;
    case GLFW_KEY_W:
        return tektite::keyboard::W;
    case GLFW_KEY_X:
        return tektite::keyboard::X;
    case GLFW_KEY_Y:
        return tektite::keyboard::Y;
    case GLFW_KEY_Z:
        return tektite::keyboard::Z;
    case GLFW_KEY_0:
        return tektite::keyboard::Num0;
    case GLFW_KEY_1:
        return tektite::keyboard::Num1;
    case GLFW_KEY_2:
        return tektite::keyboard::Num2;
    case GLFW_KEY_3:
        return tektite::keyboard::Num3;
    case GLFW_KEY_4:
        return tektite::keyboard::Num4;
    case GLFW_KEY_5:
        return tektite::keyboard::Num5;
    case GLFW_KEY_6:
        return tektite::keyboard::Num6;
    case GLFW_KEY_7:
        return tektite::keyboard::Num7;
    case GLFW_KEY_8:
        return tektite::keyboard::Num8;
    case GLFW_KEY_9:
        return tektite::keyboard::Num9;
    case GLFW_KEY_F1:
        return tektite::keyboard::F1;
    case GLFW_KEY_F2:
        return tektite::keyboard::F2;
    case GLFW_KEY_F3:
        return tektite::keyboard::F3;
    case GLFW_KEY_F4:
        return tektite::keyboard::F4;
    case GLFW_KEY_F5:
        return tektite::keyboard::F5;
    case GLFW_KEY_F6:
        return tektite::keyboard::F6;
    case GLFW_KEY_F7:
        return tektite::keyboard::F7;
    case GLFW_KEY_F8:
        return tektite::keyboard::F8;
    case GLFW_KEY_F9:
        return tektite::keyboard::F9;
    case GLFW_KEY_F10:
        return tektite::keyboard::F10;
    case GLFW_KEY_F11:
        return tektite::keyboard::F11;
    case GLFW_KEY_F12:
        return tektite::keyboard::F12;
    }

    return tektite::keyboard::Unknown;
}
