cmake_minimum_required(VERSION 3.20...3.28)

if(${CMAKE_VERSION} VERSION_LESS 3.20)
    cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif()

project(Paper)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(BUILD_SHARED_LIBS OFF)

if(CMAKE_SYSTEM_NAME STREQUAL "Emscripten")
    set(PLATFORM_EMSCRIPTEN ON)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Windows" OR 
        CMAKE_SYSTEM_NAME STREQUAL "Linux" OR
        CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    set(PLATFORM_DESKTOP ON)
endif()

add_subdirectory(third_party)
add_subdirectory(engine)
add_subdirectory(runtime)
