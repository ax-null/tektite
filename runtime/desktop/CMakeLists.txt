add_executable(tektite_desktop
    "source/main.cpp"
    $<$<PLATFORM_ID:Windows>:win32_resources.rc>
)

target_include_directories(tektite_desktop PRIVATE "source")
target_link_libraries(tektite_desktop PRIVATE
    tektite_core glfw
)
