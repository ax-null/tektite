#include <core/engine.hpp>

#include <functional>
#include <iostream>

#include <emscripten.h>
#include <emscripten/html5.h>

std::function<void()> loop, click;

void main_loop() { loop(); }

EM_BOOL mouse_click(int eventType, const EmscriptenMouseEvent *mouseEvent, void *userData)
{
    click();
    return true;
}

int main(int argc, char **argv)
{
    std::cout << "Tektite's Emscripten runtime!" << std::endl;

    emscripten_set_window_title("Tektite");
    emscripten_set_canvas_element_size("#canvas", 640, 480);

    EmscriptenWebGLContextAttributes attributes = {
        true, true, true, true, true, true,
        EM_WEBGL_POWER_PREFERENCE_DEFAULT,
        false, 2, 0, true, false,
        EMSCRIPTEN_WEBGL_CONTEXT_PROXY_DISALLOW, false
    };

    EMSCRIPTEN_WEBGL_CONTEXT_HANDLE context = emscripten_webgl_create_context("#canvas", &attributes);
    emscripten_webgl_make_context_current(context);

    Tektite::Engine *engine = new Tektite::Engine();

    click = [&] {
        engine->click();
    };

    loop = [&] {
        engine->render();
        emscripten_webgl_commit_frame();
    };

    emscripten_set_click_callback_on_thread("#canvas", nullptr, false, mouse_click, 0);
    emscripten_set_main_loop(main_loop, 0, true);
    emscripten_set_main_loop_timing(EM_TIMING_RAF, 1);

    emscripten_webgl_destroy_context(context);

    delete engine;

    return 0;
}
