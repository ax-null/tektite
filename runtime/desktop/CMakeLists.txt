add_executable(paper_desktop
    "source/main.cpp"
    $<$<PLATFORM_ID:Windows>:windows_resources.rc>
)

target_include_directories(paper_desktop PRIVATE "source")
target_link_libraries(paper_desktop PRIVATE
    paper_engine glfw
)
