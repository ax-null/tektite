add_library(paper_engine STATIC
    "source/audio/audio_engine.cpp"

    "source/graphics/camera.cpp"
    "source/graphics/font.cpp"
    "source/graphics/graphics.cpp"
    "source/graphics/texture.cpp"

    "source/input/input_system.cpp"

    "source/script/module/audio_module.cpp"
    "source/script/module/graphics_module.cpp"
    "source/script/module/input_module.cpp"
    "source/script/module/world_module.cpp"
	
    "source/script/object/entity_object.cpp"
    "source/script/object/font_object.cpp"
    "source/script/object/texture_object.cpp"

    "source/script/script_engine.cpp"

    "source/third_party/miniaudio.c"
    
    "source/world/entity.cpp"
    "source/world/tilemap.cpp"
    "source/world/world.cpp"

    "source/engine.cpp"
)

target_include_directories(paper_engine PUBLIC "include" PRIVATE "source")
target_link_libraries(paper_engine PUBLIC 
    glm lua jsoncpp_static $<$<BOOL:${PLATFORM_DESKTOP}>:glad>
)
