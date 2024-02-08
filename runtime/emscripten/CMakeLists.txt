set(CMAKE_EXECUTABLE_SUFFIX ".html")

set(ASSETS_PATH "unspecified" CACHE STRING "The location of the assets directory to be bundled.")

add_executable(paper_emscripten
    "source/main.cpp"
)

target_include_directories(paper_emscripten PRIVATE "source")
target_link_libraries(paper_emscripten 
	paper_engine 
	"-sAUDIO_WORKLET -sWASM_WORKERS -sASYNCIFY --emrun
	--preload-file ${ASSETS_PATH}@assets"
)
