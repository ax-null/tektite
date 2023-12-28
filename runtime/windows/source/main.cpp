#include <core/engine/engine.hpp>

#include <iostream>

#define WIN32_LEAN_AND_MEAN
#include <glad/wgl.h>
#include <windows.h>

struct WindowsState
{
    WNDCLASSEXA windowClass;

    HWND window;
    HGLRC context;
    MSG message;

    uint32_t width, height;
    Tektite::Engine *engine;

    bool shouldQuit;
} static state;

bool initializeWGL(LPCSTR wndClass, HINSTANCE hInstance);
Tektite::Keyboard::Key translateKey(WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg) {
    case WM_ERASEBKGND:
        return 1;
    case WM_CLOSE:
        state.shouldQuit = true;
        return 0;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }

    if (state.engine != nullptr) {
        Tektite::Input *input = state.engine->getInput();

        switch (msg) {
        case WM_KEYDOWN:
        case WM_SYSKEYDOWN:
        case WM_KEYUP:
        case WM_SYSKEYUP:
        {
            bool down = (msg == WM_KEYDOWN) || (msg == WM_SYSKEYDOWN);
            Tektite::Keyboard::Key key = translateKey(wParam, lParam);
            if (down)
                input->onPress(key);
            else
                input->onRelease(key);
            return 0;
        }
        case WM_MOUSEMOVE:
            input->onMove((float)((uint16_t)lParam), (float)(lParam >> 16));
            break;
        case WM_LBUTTONDBLCLK:
        case WM_MBUTTONDBLCLK:
        case WM_RBUTTONDBLCLK:
        case WM_LBUTTONDOWN:
        case WM_MBUTTONDOWN:
        case WM_RBUTTONDOWN:
        case WM_LBUTTONUP:
        case WM_MBUTTONUP:
        case WM_RBUTTONUP:
        {
            // TODO: keep mouse input even when the cursor is out of the window.
            bool down = (msg == WM_LBUTTONDOWN) || (msg == WM_MBUTTONDOWN) || (msg == WM_RBUTTONDOWN) ||
                        (msg == WM_LBUTTONDBLCLK) || (msg == WM_MBUTTONDBLCLK) || (msg == WM_RBUTTONDBLCLK);

            Tektite::Mouse::Button button = Tektite::Mouse::Unknown;

            switch (msg) {
            case WM_LBUTTONDBLCLK:
            case WM_LBUTTONDOWN:
            case WM_LBUTTONUP:
                button = Tektite::Mouse::Left;
                break;
            case WM_MBUTTONDBLCLK:
            case WM_MBUTTONDOWN:
            case WM_MBUTTONUP:
                button = Tektite::Mouse::Middle;
                break;
            case WM_RBUTTONDBLCLK:
            case WM_RBUTTONDOWN:
            case WM_RBUTTONUP:
                button = Tektite::Mouse::Right;
                break;
            }

            if (down)
                input->onPress(button);
            else
                input->onRelease(button);

            return 0;
        }
        }
    }

    return DefWindowProc(hWnd, msg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, INT nCmdShow)
{
    state = WindowsState();
    state.shouldQuit = false;

    state.windowClass = { sizeof(state.windowClass) };
    state.windowClass.lpszClassName = "TEKWNDCLS";
    state.windowClass.hInstance = hInstance;
    state.windowClass.lpfnWndProc = WndProc;
    state.windowClass.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
    state.windowClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
    state.windowClass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(460));
    state.windowClass.hbrBackground = nullptr;

    RegisterClassEx(&state.windowClass);

    state.width = 800;
    state.height = 600;

    if ((state.window = CreateWindowExA(0, state.windowClass.lpszClassName, "Tektite",
                                        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_VISIBLE,
                                        CW_USEDEFAULT, CW_USEDEFAULT, state.width, state.height,
                                        NULL, NULL, hInstance, NULL)) == NULL)
        return -1;

    initializeWGL(state.windowClass.lpszClassName, hInstance);

    HDC hDC = GetDC(state.window);

    int pixel_format_attribs[] = {
        WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
        WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
        WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
        WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB,
        WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
        WGL_COLOR_BITS_ARB, 32,
        WGL_DEPTH_BITS_ARB, 24,
        WGL_STENCIL_BITS_ARB, 8, 0
    };

    int pixel_format;
    UINT num_formats;
    wglChoosePixelFormatARB(hDC, pixel_format_attribs, 0, 1, &pixel_format, &num_formats);

    if (!num_formats) {
        std::cout << "No opengl pixel formats found.\n";
        return -1;
    }

    PIXELFORMATDESCRIPTOR pfd;
    DescribePixelFormat(hDC, pixel_format, sizeof(pfd), &pfd);

    if (!SetPixelFormat(hDC, pixel_format, &pfd)) {
        std::cout << "Failed to set the OpenGL pixel format.\n";
        return -1;
    }

    int glAttribs[] = {
        WGL_CONTEXT_MAJOR_VERSION_ARB, 2,
        WGL_CONTEXT_MINOR_VERSION_ARB, 1,
        WGL_CONTEXT_PROFILE_MASK_ARB,
        WGL_CONTEXT_CORE_PROFILE_BIT_ARB, 0
    };

    state.context = wglCreateContextAttribsARB(hDC, nullptr, glAttribs);

    wglMakeContextCurrentARB(hDC, hDC, state.context);
    wglSwapIntervalEXT(1);

    gladLoaderLoadGL();

    state.engine = new Tektite::Engine();

    while (state.engine->isRunning() && !state.shouldQuit) {
        if (PeekMessage(&state.message, NULL, 0U, 0U, PM_REMOVE)) {
            ::TranslateMessage(&state.message);
            ::DispatchMessage(&state.message);
        }

        state.engine->run();
        SwapBuffers(hDC);
    };

    delete state.engine;

    wglDeleteContext(state.context);
    DestroyWindow(state.window);

    UnregisterClass(state.windowClass.lpszClassName, hInstance);

    return 0;
}

bool initializeWGL(LPCSTR wndClass, HINSTANCE hInstance)
{
    HWND dummyWindow = CreateWindowExA(0, wndClass, "Dummy", 0,
                                       CW_USEDEFAULT, CW_USEDEFAULT,
                                       CW_USEDEFAULT, CW_USEDEFAULT,
                                       0, 0, hInstance, 0);

    if (!dummyWindow) {
        std::cout << "Failed to create dummy OpenGL window.\n";
        return false;
    }

    HDC dummyHDC = GetDC(dummyWindow);

    PIXELFORMATDESCRIPTOR pfd = {
        sizeof(pfd),
        1,
        PFD_TYPE_RGBA,
        PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
        32,
        8,
        PFD_MAIN_PLANE,
        24,
        8,
    };

    int pixelFormat = ChoosePixelFormat(dummyHDC, &pfd);

    if (!pixelFormat) {
        std::cout << "Failed to find a suitable pixel format.\n";
        return false;
    }

    if (!::SetPixelFormat(dummyHDC, pixelFormat, &pfd)) {
        std::cout << "Failed to set the pixel format.\n";
        return false;
    }

    HGLRC dummyContext = wglCreateContext(dummyHDC);

    if (!dummyContext) {
        std::cout << "Failed to create a dummy OpenGL rendering context.\n";
        return false;
    }

    if (!::wglMakeCurrent(dummyHDC, dummyContext)) {
        std::cout << "Failed to activate dummy OpenGL rendering context.\n";
        return false;
    }

    gladLoaderLoadWGL(dummyHDC);

    wglMakeCurrent(dummyHDC, 0);
    wglDeleteContext(dummyContext);
    ReleaseDC(dummyWindow, dummyHDC);
    DestroyWindow(dummyWindow);

    return true;
}

Tektite::Keyboard::Key translateKey(WPARAM wParam, LPARAM lParam)
{
    switch (wParam) {
    case VK_UP:
        return Tektite::Keyboard::Up;
    case VK_DOWN:
        return Tektite::Keyboard::Down;
    case VK_LEFT:
        return Tektite::Keyboard::Left;
    case VK_RIGHT:
        return Tektite::Keyboard::Right;
    case VK_ESCAPE:
        return Tektite::Keyboard::Escape;
    case VK_RETURN:
        return Tektite::Keyboard::Enter;
    case VK_BACK:
        return Tektite::Keyboard::Backspace;
    case VK_HOME:
        return Tektite::Keyboard::Home;
    case VK_INSERT:
        return Tektite::Keyboard::Insert;
    case VK_PRIOR:
        return Tektite::Keyboard::PageUp;
    case VK_NEXT:
        return Tektite::Keyboard::PageDown;
    case VK_DELETE:
        return Tektite::Keyboard::Delete;
    case VK_END:
        return Tektite::Keyboard::End;
    case VK_PRINT:
        return Tektite::Keyboard::PrintScreen;
    case VK_PAUSE:
        return Tektite::Keyboard::Pause;
    case VK_SCROLL:
        return Tektite::Keyboard::ScrollLock;
    case VK_NUMLOCK:
        return Tektite::Keyboard::NumLock;
    case VK_CAPITAL:
        return Tektite::Keyboard::CapsLock;
    case VK_TAB:
        return Tektite::Keyboard::Tab;
    case VK_SPACE:
        return Tektite::Keyboard::Space;
    case VK_LSHIFT:
        return Tektite::Keyboard::LeftShift;
    case VK_RSHIFT:
        return Tektite::Keyboard::RightShift;
    case VK_LCONTROL:
        return Tektite::Keyboard::LeftCtrl;
    case VK_RCONTROL:
        return Tektite::Keyboard::RightCtrl;
    case VK_LMENU:
        return Tektite::Keyboard::LeftAlt;
    case VK_RMENU:
        return Tektite::Keyboard::RightAlt;
    case VK_NUMPAD0:
        return Tektite::Keyboard::Numpad0;
    case VK_NUMPAD1:
        return Tektite::Keyboard::Numpad1;
    case VK_NUMPAD2:
        return Tektite::Keyboard::Numpad2;
    case VK_NUMPAD3:
        return Tektite::Keyboard::Numpad3;
    case VK_NUMPAD4:
        return Tektite::Keyboard::Numpad4;
    case VK_NUMPAD5:
        return Tektite::Keyboard::Numpad5;
    case VK_NUMPAD6:
        return Tektite::Keyboard::Numpad6;
    case VK_NUMPAD7:
        return Tektite::Keyboard::Numpad7;
    case VK_NUMPAD8:
        return Tektite::Keyboard::Numpad8;
    case VK_NUMPAD9:
        return Tektite::Keyboard::Numpad9;
    case VK_F1:
        return Tektite::Keyboard::F1;
    case VK_F2:
        return Tektite::Keyboard::F2;
    case VK_F3:
        return Tektite::Keyboard::F3;
    case VK_F4:
        return Tektite::Keyboard::F4;
    case VK_F5:
        return Tektite::Keyboard::F5;
    case VK_F6:
        return Tektite::Keyboard::F6;
    case VK_F7:
        return Tektite::Keyboard::F7;
    case VK_F8:
        return Tektite::Keyboard::F8;
    case VK_F9:
        return Tektite::Keyboard::F9;
    case VK_F10:
        return Tektite::Keyboard::F10;
    case VK_F11:
        return Tektite::Keyboard::F11;
    case VK_F12:
        return Tektite::Keyboard::F12;
    }

    switch ((lParam >> 16) & 0xff) {
    case 43:
        return Tektite::Keyboard::Backslash;
    case 30:
        return Tektite::Keyboard::A;
    case 48:
        return Tektite::Keyboard::B;
    case 46:
        return Tektite::Keyboard::C;
    case 32:
        return Tektite::Keyboard::D;
    case 18:
        return Tektite::Keyboard::E;
    case 33:
        return Tektite::Keyboard::F;
    case 34:
        return Tektite::Keyboard::G;
    case 35:
        return Tektite::Keyboard::H;
    case 23:
        return Tektite::Keyboard::I;
    case 36:
        return Tektite::Keyboard::J;
    case 37:
        return Tektite::Keyboard::K;
    case 38:
        return Tektite::Keyboard::L;
    case 50:
        return Tektite::Keyboard::M;
    case 49:
        return Tektite::Keyboard::N;
    case 24:
        return Tektite::Keyboard::O;
    case 25:
        return Tektite::Keyboard::P;
    case 16:
        return Tektite::Keyboard::Q;
    case 19:
        return Tektite::Keyboard::R;
    case 31:
        return Tektite::Keyboard::S;
    case 20:
        return Tektite::Keyboard::T;
    case 22:
        return Tektite::Keyboard::U;
    case 47:
        return Tektite::Keyboard::V;
    case 17:
        return Tektite::Keyboard::W;
    case 45:
        return Tektite::Keyboard::X;
    case 21:
        return Tektite::Keyboard::Y;
    case 44:
        return Tektite::Keyboard::Z;
    case 11:
        return Tektite::Keyboard::Num0;
    case 2:
        return Tektite::Keyboard::Num1;
    case 3:
        return Tektite::Keyboard::Num2;
    case 4:
        return Tektite::Keyboard::Num3;
    case 5:
        return Tektite::Keyboard::Num4;
    case 6:
        return Tektite::Keyboard::Num5;
    case 7:
        return Tektite::Keyboard::Num6;
    case 8:
        return Tektite::Keyboard::Num7;
    case 9:
        return Tektite::Keyboard::Num8;
    case 10:
        return Tektite::Keyboard::Num9;
    }

    return Tektite::Keyboard::Unknown;
}
