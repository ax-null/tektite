#include <core/engine/engine.hpp>

#include <stdio.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <glad/gl.h>

struct DesktopState
{
    GLFWwindow *window;
    int32_t width, height;
    Tektite::Engine *engine;
} static state;

Tektite::Keyboard::Key translateKey(int keyCode);

void errorCallback(int error, const char *description)
{
    fprintf(stderr, "Error: %s\n", description);
}

void windowSizeCallback(GLFWwindow *window, int width, int height)
{
    if (state.engine != nullptr)
        state.engine->getGraphics()->resize(width, height);
}

void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS)
        state.engine->getInput()->onPress(translateKey(key));
    else if (action == GLFW_RELEASE)
        state.engine->getInput()->onRelease(translateKey(key));
}

void mouseButtonCallback(GLFWwindow *window, int button, int action, int mods)
{
    Tektite::Mouse::Button mouseButton;

    switch (button) {
    case GLFW_MOUSE_BUTTON_LEFT:
        mouseButton = Tektite::Mouse::Left;
        break;
    case GLFW_MOUSE_BUTTON_MIDDLE:
        mouseButton = Tektite::Mouse::Middle;
        break;
    case GLFW_MOUSE_BUTTON_RIGHT:
        mouseButton = Tektite::Mouse::Right;
        break;
    default:
        mouseButton = Tektite::Mouse::Unknown;
    }

    if (action == GLFW_PRESS)
        state.engine->getInput()->onPress(mouseButton);
    else if (action == GLFW_RELEASE)
        state.engine->getInput()->onRelease(mouseButton);
}

void cursorPositionCallback(GLFWwindow *window, double xpos, double ypos)
{
    state.engine->getInput()->onMove(xpos, ypos);
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

    glfwSetErrorCallback(errorCallback);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    state.window = glfwCreateWindow(state.width, state.height, "Tektite", nullptr, nullptr);

    if (!state.window) {
        printf("Couldn't create window.\n");
        return -1;
    }

    const GLFWvidmode *videoMode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    glfwSetWindowPos(state.window,
                     (videoMode->width - state.width) / 2,
                     (videoMode->height - state.height) / 2);

    glfwSetWindowSizeCallback(state.window, windowSizeCallback);

    glfwSetKeyCallback(state.window, keyCallback);
    glfwSetMouseButtonCallback(state.window, mouseButtonCallback);
    glfwSetCursorPosCallback(state.window, cursorPositionCallback);

    glfwMakeContextCurrent(state.window);
    glfwSwapInterval(1);

    gladLoadGL(glfwGetProcAddress);

    state.engine = new Tektite::Engine();
    state.engine->getGraphics()->resize(800, 600);

    while (state.engine->isRunning() && !glfwWindowShouldClose(state.window)) {
        glfwPollEvents();

        state.engine->run();
        glfwSwapBuffers(state.window);
    }

    delete state.engine;

    glfwDestroyWindow(state.window);
    glfwTerminate();

    return 0;
}

Tektite::Keyboard::Key translateKey(int keyCode)
{
    switch (keyCode) {
    case GLFW_KEY_UP:
        return Tektite::Keyboard::Up;
    case GLFW_KEY_DOWN:
        return Tektite::Keyboard::Down;
    case GLFW_KEY_LEFT:
        return Tektite::Keyboard::Left;
    case GLFW_KEY_RIGHT:
        return Tektite::Keyboard::Right;
    case GLFW_KEY_ESCAPE:
        return Tektite::Keyboard::Escape;
    case GLFW_KEY_ENTER:
        return Tektite::Keyboard::Enter;
    case GLFW_KEY_BACKSPACE:
        return Tektite::Keyboard::Backspace;
    case GLFW_KEY_HOME:
        return Tektite::Keyboard::Home;
    case GLFW_KEY_INSERT:
        return Tektite::Keyboard::Insert;
    case GLFW_KEY_PAGE_UP:
        return Tektite::Keyboard::PageUp;
    case GLFW_KEY_PAGE_DOWN:
        return Tektite::Keyboard::PageDown;
    case GLFW_KEY_DELETE:
        return Tektite::Keyboard::Delete;
    case GLFW_KEY_END:
        return Tektite::Keyboard::End;
    case GLFW_KEY_PRINT_SCREEN:
        return Tektite::Keyboard::PrintScreen;
    case GLFW_KEY_PAUSE:
        return Tektite::Keyboard::Pause;
    case GLFW_KEY_SCROLL_LOCK:
        return Tektite::Keyboard::ScrollLock;
    case GLFW_KEY_NUM_LOCK:
        return Tektite::Keyboard::NumLock;
    case GLFW_KEY_CAPS_LOCK:
        return Tektite::Keyboard::CapsLock;
    case GLFW_KEY_TAB:
        return Tektite::Keyboard::Tab;
    case GLFW_KEY_BACKSLASH:
        return Tektite::Keyboard::Backslash;
    case GLFW_KEY_SPACE:
        return Tektite::Keyboard::Space;
    case GLFW_KEY_LEFT_SHIFT:
        return Tektite::Keyboard::LeftShift;
    case GLFW_KEY_RIGHT_SHIFT:
        return Tektite::Keyboard::RightShift;
    case GLFW_KEY_LEFT_CONTROL:
        return Tektite::Keyboard::LeftCtrl;
    case GLFW_KEY_RIGHT_CONTROL:
        return Tektite::Keyboard::RightCtrl;
    case GLFW_KEY_LEFT_ALT:
        return Tektite::Keyboard::LeftAlt;
    case GLFW_KEY_RIGHT_ALT:
        return Tektite::Keyboard::RightAlt;
    case GLFW_KEY_KP_0:
        return Tektite::Keyboard::Numpad0;
    case GLFW_KEY_KP_1:
        return Tektite::Keyboard::Numpad1;
    case GLFW_KEY_KP_2:
        return Tektite::Keyboard::Numpad2;
    case GLFW_KEY_KP_3:
        return Tektite::Keyboard::Numpad3;
    case GLFW_KEY_KP_4:
        return Tektite::Keyboard::Numpad4;
    case GLFW_KEY_KP_5:
        return Tektite::Keyboard::Numpad5;
    case GLFW_KEY_KP_6:
        return Tektite::Keyboard::Numpad6;
    case GLFW_KEY_KP_7:
        return Tektite::Keyboard::Numpad7;
    case GLFW_KEY_KP_8:
        return Tektite::Keyboard::Numpad8;
    case GLFW_KEY_KP_9:
        return Tektite::Keyboard::Numpad9;
    case GLFW_KEY_A:
        return Tektite::Keyboard::A;
    case GLFW_KEY_B:
        return Tektite::Keyboard::B;
    case GLFW_KEY_C:
        return Tektite::Keyboard::C;
    case GLFW_KEY_D:
        return Tektite::Keyboard::D;
    case GLFW_KEY_E:
        return Tektite::Keyboard::E;
    case GLFW_KEY_F:
        return Tektite::Keyboard::F;
    case GLFW_KEY_G:
        return Tektite::Keyboard::G;
    case GLFW_KEY_H:
        return Tektite::Keyboard::H;
    case GLFW_KEY_I:
        return Tektite::Keyboard::I;
    case GLFW_KEY_J:
        return Tektite::Keyboard::J;
    case GLFW_KEY_K:
        return Tektite::Keyboard::K;
    case GLFW_KEY_L:
        return Tektite::Keyboard::L;
    case GLFW_KEY_M:
        return Tektite::Keyboard::M;
    case GLFW_KEY_N:
        return Tektite::Keyboard::N;
    case GLFW_KEY_O:
        return Tektite::Keyboard::O;
    case GLFW_KEY_P:
        return Tektite::Keyboard::P;
    case GLFW_KEY_Q:
        return Tektite::Keyboard::Q;
    case GLFW_KEY_R:
        return Tektite::Keyboard::R;
    case GLFW_KEY_S:
        return Tektite::Keyboard::S;
    case GLFW_KEY_T:
        return Tektite::Keyboard::T;
    case GLFW_KEY_U:
        return Tektite::Keyboard::U;
    case GLFW_KEY_V:
        return Tektite::Keyboard::V;
    case GLFW_KEY_W:
        return Tektite::Keyboard::W;
    case GLFW_KEY_X:
        return Tektite::Keyboard::X;
    case GLFW_KEY_Y:
        return Tektite::Keyboard::Y;
    case GLFW_KEY_Z:
        return Tektite::Keyboard::Z;
    case GLFW_KEY_0:
        return Tektite::Keyboard::Num0;
    case GLFW_KEY_1:
        return Tektite::Keyboard::Num1;
    case GLFW_KEY_2:
        return Tektite::Keyboard::Num2;
    case GLFW_KEY_3:
        return Tektite::Keyboard::Num3;
    case GLFW_KEY_4:
        return Tektite::Keyboard::Num4;
    case GLFW_KEY_5:
        return Tektite::Keyboard::Num5;
    case GLFW_KEY_6:
        return Tektite::Keyboard::Num6;
    case GLFW_KEY_7:
        return Tektite::Keyboard::Num7;
    case GLFW_KEY_8:
        return Tektite::Keyboard::Num8;
    case GLFW_KEY_9:
        return Tektite::Keyboard::Num9;
    case GLFW_KEY_F1:
        return Tektite::Keyboard::F1;
    case GLFW_KEY_F2:
        return Tektite::Keyboard::F2;
    case GLFW_KEY_F3:
        return Tektite::Keyboard::F3;
    case GLFW_KEY_F4:
        return Tektite::Keyboard::F4;
    case GLFW_KEY_F5:
        return Tektite::Keyboard::F5;
    case GLFW_KEY_F6:
        return Tektite::Keyboard::F6;
    case GLFW_KEY_F7:
        return Tektite::Keyboard::F7;
    case GLFW_KEY_F8:
        return Tektite::Keyboard::F8;
    case GLFW_KEY_F9:
        return Tektite::Keyboard::F9;
    case GLFW_KEY_F10:
        return Tektite::Keyboard::F10;
    case GLFW_KEY_F11:
        return Tektite::Keyboard::F11;
    case GLFW_KEY_F12:
        return Tektite::Keyboard::F12;
    }

    return Tektite::Keyboard::Unknown;
}
