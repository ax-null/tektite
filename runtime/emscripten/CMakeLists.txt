set(CMAKE_EXECUTABLE_SUFFIX ".html")

add_executable(tektite_emscripten
    "source/main.cpp"
)

target_include_directories(tektite_emscripten PRIVATE "source")
target_link_libraries(tektite_emscripten 
	tektite_core 
	"-sAUDIO_WORKLET -sWASM_WORKERS -sASYNCIFY --emrun
	--preload-file D:/Dev/Private/Tektite/assets@assets"
)
