if(PLATFORM_EMSCRIPTEN)
	add_subdirectory(emscripten)
endif()

if(PLATFORM_DESKTOP)
	add_subdirectory(desktop)
endif()
