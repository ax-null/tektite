if(WIN32)
	add_subdirectory(windows)
endif()

if(EMSCRIPTEN)
	add_subdirectory(emscripten)
endif()
