#include <engine/engine.hpp>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/gl.h>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>
#elif _POSIX_C_SOURCE >= 199309L
#include <unistd.h>
#endif

struct DesktopState
{
    paper::Engine *engine;
    paper::time::nano current_time;

    GLFWwindow *window;
    int32_t width, height;
} static state;

paper::keyboard::Key translate_key(int keyCode);

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
    paper::mouse::Button mouse_button;

    switch (button) {
    case GLFW_MOUSE_BUTTON_LEFT:
        mouse_button = paper::mouse::Left;
        break;
    case GLFW_MOUSE_BUTTON_MIDDLE:
        mouse_button = paper::mouse::Middle;
        break;
    case GLFW_MOUSE_BUTTON_RIGHT:
        mouse_button = paper::mouse::Right;
        break;
    default:
        mouse_button = paper::mouse::Unknown;
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
    state.window = glfwCreateWindow(state.width, state.height, "Paper", nullptr, nullptr);

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
    gladLoadGL(glfwGetProcAddress);

    state.engine = new paper::Engine();
    state.engine->get_graphics()->resize(state.width, state.height);

    while (state.engine->is_running() && !glfwWindowShouldClose(state.window)) {
        glfwPollEvents();

        state.current_time = paper::time::from_seconds(glfwGetTime());
        state.engine->run(state.current_time);

        glfwSwapInterval(state.engine->is_vsync_enabled());
        glfwSwapBuffers(state.window);

#ifdef _WIN32
        Sleep(paper::time::to_milliseconds(state.engine->get_sleep_time()));
#elif _POSIX_C_SOURCE >= 199309L
        usleep(paper::time::to_milliseconds(state.engine->get_sleep_time()) * 1000);
#endif
    }

    delete state.engine;

    glfwDestroyWindow(state.window);
    glfwTerminate();

    return 0;
}

paper::keyboard::Key translate_key(int keyCode)
{
    switch (keyCode) {
    case GLFW_KEY_UP:
        return paper::keyboard::Up;
    case GLFW_KEY_DOWN:
        return paper::keyboard::Down;
    case GLFW_KEY_LEFT:
        return paper::keyboard::Left;
    case GLFW_KEY_RIGHT:
        return paper::keyboard::Right;
    case GLFW_KEY_ESCAPE:
        return paper::keyboard::Escape;
    case GLFW_KEY_ENTER:
        return paper::keyboard::Enter;
    case GLFW_KEY_BACKSPACE:
        return paper::keyboard::Backspace;
    case GLFW_KEY_HOME:
        return paper::keyboard::Home;
    case GLFW_KEY_INSERT:
        return paper::keyboard::Insert;
    case GLFW_KEY_PAGE_UP:
        return paper::keyboard::PageUp;
    case GLFW_KEY_PAGE_DOWN:
        return paper::keyboard::PageDown;
    case GLFW_KEY_DELETE:
        return paper::keyboard::Delete;
    case GLFW_KEY_END:
        return paper::keyboard::End;
    case GLFW_KEY_PRINT_SCREEN:
        return paper::keyboard::PrintScreen;
    case GLFW_KEY_PAUSE:
        return paper::keyboard::Pause;
    case GLFW_KEY_SCROLL_LOCK:
        return paper::keyboard::ScrollLock;
    case GLFW_KEY_NUM_LOCK:
        return paper::keyboard::NumLock;
    case GLFW_KEY_CAPS_LOCK:
        return paper::keyboard::CapsLock;
    case GLFW_KEY_TAB:
        return paper::keyboard::Tab;
    case GLFW_KEY_BACKSLASH:
        return paper::keyboard::Backslash;
    case GLFW_KEY_SPACE:
        return paper::keyboard::Space;
    case GLFW_KEY_LEFT_SHIFT:
        return paper::keyboard::LeftShift;
    case GLFW_KEY_RIGHT_SHIFT:
        return paper::keyboard::RightShift;
    case GLFW_KEY_LEFT_CONTROL:
        return paper::keyboard::LeftCtrl;
    case GLFW_KEY_RIGHT_CONTROL:
        return paper::keyboard::RightCtrl;
    case GLFW_KEY_LEFT_ALT:
        return paper::keyboard::LeftAlt;
    case GLFW_KEY_RIGHT_ALT:
        return paper::keyboard::RightAlt;
    case GLFW_KEY_KP_0:
        return paper::keyboard::Numpad0;
    case GLFW_KEY_KP_1:
        return paper::keyboard::Numpad1;
    case GLFW_KEY_KP_2:
        return paper::keyboard::Numpad2;
    case GLFW_KEY_KP_3:
        return paper::keyboard::Numpad3;
    case GLFW_KEY_KP_4:
        return paper::keyboard::Numpad4;
    case GLFW_KEY_KP_5:
        return paper::keyboard::Numpad5;
    case GLFW_KEY_KP_6:
        return paper::keyboard::Numpad6;
    case GLFW_KEY_KP_7:
        return paper::keyboard::Numpad7;
    case GLFW_KEY_KP_8:
        return paper::keyboard::Numpad8;
    case GLFW_KEY_KP_9:
        return paper::keyboard::Numpad9;
    case GLFW_KEY_A:
        return paper::keyboard::A;
    case GLFW_KEY_B:
        return paper::keyboard::B;
    case GLFW_KEY_C:
        return paper::keyboard::C;
    case GLFW_KEY_D:
        return paper::keyboard::D;
    case GLFW_KEY_E:
        return paper::keyboard::E;
    case GLFW_KEY_F:
        return paper::keyboard::F;
    case GLFW_KEY_G:
        return paper::keyboard::G;
    case GLFW_KEY_H:
        return paper::keyboard::H;
    case GLFW_KEY_I:
        return paper::keyboard::I;
    case GLFW_KEY_J:
        return paper::keyboard::J;
    case GLFW_KEY_K:
        return paper::keyboard::K;
    case GLFW_KEY_L:
        return paper::keyboard::L;
    case GLFW_KEY_M:
        return paper::keyboard::M;
    case GLFW_KEY_N:
        return paper::keyboard::N;
    case GLFW_KEY_O:
        return paper::keyboard::O;
    case GLFW_KEY_P:
        return paper::keyboard::P;
    case GLFW_KEY_Q:
        return paper::keyboard::Q;
    case GLFW_KEY_R:
        return paper::keyboard::R;
    case GLFW_KEY_S:
        return paper::keyboard::S;
    case GLFW_KEY_T:
        return paper::keyboard::T;
    case GLFW_KEY_U:
        return paper::keyboard::U;
    case GLFW_KEY_V:
        return paper::keyboard::V;
    case GLFW_KEY_W:
        return paper::keyboard::W;
    case GLFW_KEY_X:
        return paper::keyboard::X;
    case GLFW_KEY_Y:
        return paper::keyboard::Y;
    case GLFW_KEY_Z:
        return paper::keyboard::Z;
    case GLFW_KEY_0:
        return paper::keyboard::Num0;
    case GLFW_KEY_1:
        return paper::keyboard::Num1;
    case GLFW_KEY_2:
        return paper::keyboard::Num2;
    case GLFW_KEY_3:
        return paper::keyboard::Num3;
    case GLFW_KEY_4:
        return paper::keyboard::Num4;
    case GLFW_KEY_5:
        return paper::keyboard::Num5;
    case GLFW_KEY_6:
        return paper::keyboard::Num6;
    case GLFW_KEY_7:
        return paper::keyboard::Num7;
    case GLFW_KEY_8:
        return paper::keyboard::Num8;
    case GLFW_KEY_9:
        return paper::keyboard::Num9;
    case GLFW_KEY_F1:
        return paper::keyboard::F1;
    case GLFW_KEY_F2:
        return paper::keyboard::F2;
    case GLFW_KEY_F3:
        return paper::keyboard::F3;
    case GLFW_KEY_F4:
        return paper::keyboard::F4;
    case GLFW_KEY_F5:
        return paper::keyboard::F5;
    case GLFW_KEY_F6:
        return paper::keyboard::F6;
    case GLFW_KEY_F7:
        return paper::keyboard::F7;
    case GLFW_KEY_F8:
        return paper::keyboard::F8;
    case GLFW_KEY_F9:
        return paper::keyboard::F9;
    case GLFW_KEY_F10:
        return paper::keyboard::F10;
    case GLFW_KEY_F11:
        return paper::keyboard::F11;
    case GLFW_KEY_F12:
        return paper::keyboard::F12;
    }

    return paper::keyboard::Unknown;
}
