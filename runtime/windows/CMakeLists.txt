add_executable(tektite_windows WIN32
    "source/main.cpp"
	"resources.rc"
)

target_include_directories(tektite_windows PRIVATE "source")
target_link_libraries(tektite_windows PRIVATE
    tektite_core opengl32 xinput dwmapi
)
