#include <core/engine.hpp>

#include <iostream>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <glad/wgl.h>

static bool running = true, click = false;

bool InitializeWGL(LPCSTR wndClass, HINSTANCE hInstance);

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg) {
	case WM_ERASEBKGND:
		return 1;
	case WM_CLOSE:
		running = false;
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
    case WM_LBUTTONDOWN:
        click = true;
        return 0;
    }

    return DefWindowProc(hWnd, msg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, INT nCmdShow)
{
    std::cout << "Tektite's Windows runtime!" << std::endl;

    WNDCLASSA window_class = {};
    window_class.lpszClassName = "TEKWNDCLS";
    window_class.hInstance = hInstance;
    window_class.lpfnWndProc = WndProc;
    window_class.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
    window_class.hCursor = LoadCursor(NULL, IDC_ARROW);
    window_class.hIcon = LoadIcon(GetModuleHandle(nullptr), MAKEINTRESOURCE(460));
    window_class.hbrBackground = NULL;

    RegisterClass(&window_class);

    HWND window = NULL;
    uint32_t window_width = 800, window_height = 600;

    if ((window = CreateWindowExA(0, window_class.lpszClassName, "Tektite",
                                  WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_VISIBLE,
                                  CW_USEDEFAULT, CW_USEDEFAULT, window_width, window_height,
                                  NULL, NULL, hInstance, NULL)) == NULL)
        return -1;

    InitializeWGL(window_class.lpszClassName, hInstance);

    HDC hDC = ::GetDC(window);

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
    ::DescribePixelFormat(hDC, pixel_format, sizeof(pfd), &pfd);

    if (!::SetPixelFormat(hDC, pixel_format, &pfd)) {
        std::cout << "Failed to set the OpenGL pixel format.\n";
        return -1;
    }

    int glAttribs[] = {
        WGL_CONTEXT_MAJOR_VERSION_ARB, 2,
        WGL_CONTEXT_MINOR_VERSION_ARB, 1,
        WGL_CONTEXT_PROFILE_MASK_ARB,
        WGL_CONTEXT_CORE_PROFILE_BIT_ARB, 0
    };

    HGLRC context = wglCreateContextAttribsARB(hDC, nullptr, glAttribs);

    wglMakeContextCurrentARB(hDC, hDC, context);
    wglSwapIntervalEXT(1);

    gladLoaderLoadGL();

    Tektite::Engine *engine = new Tektite::Engine();

    MSG message = {};
    while (running) {
        if (PeekMessage(&message, NULL, 0U, 0U, PM_REMOVE)) {
            ::TranslateMessage(&message);
            ::DispatchMessage(&message);
        }

        if (click) {
            engine->click();
            click = false;
        }

        engine->render();
        SwapBuffers(hDC);
    };

    delete engine;

    wglDeleteContext(context);
    DestroyWindow(window);

    UnregisterClassA(window_class.lpszClassName, hInstance);

    return 0;
}

bool InitializeWGL(LPCSTR wndClass, HINSTANCE hInstance)
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

    int pixelFormat = ::ChoosePixelFormat(dummyHDC, &pfd);

    if (!pixelFormat) {
        std::cout << "Failed to find a suitable pixel format.\n";
        return false;
    }

    if (!::SetPixelFormat(dummyHDC, pixelFormat, &pfd)) {
        std::cout << "Failed to set the pixel format.\n";
        return false;
    }

    HGLRC dummyContext = ::wglCreateContext(dummyHDC);

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
