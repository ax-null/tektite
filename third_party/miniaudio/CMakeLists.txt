project(miniaudio LANGUAGES C VERSION 0.11.21)

add_library(miniaudio STATIC "src/miniaudio.c")
target_include_directories(miniaudio PUBLIC "include")

if(EMSCRIPTEN)
    target_compile_options(miniaudio PUBLIC "-pthread")
endif()
