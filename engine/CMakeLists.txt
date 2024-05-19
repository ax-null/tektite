add_library(paper_engine STATIC

    "source/graphics/camera.cpp"
    "source/graphics/font.cpp"
    "source/graphics/texture.cpp"

    "source/modules/audio.cpp"
    "source/modules/graphics.cpp"
    "source/modules/input.cpp"
    "source/modules/world.cpp"
	
    "source/objects/entity.cpp"
    "source/objects/font.cpp"
    "source/objects/texture.cpp"

    "source/world/entity.cpp"
    "source/world/tilemap.cpp"

    "source/audio.cpp"
    "source/engine.cpp"
    "source/graphics.cpp"
    "source/input.cpp"
    "source/script.cpp"
    "source/world.cpp"
)

target_include_directories(paper_engine PUBLIC "include" PRIVATE "source")
target_link_libraries(paper_engine PUBLIC 
    glm $<$<BOOL:${PLATFORM_DESKTOP}>:glad> jsoncpp_static lua miniaudio stb 
)
