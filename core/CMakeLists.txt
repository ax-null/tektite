add_library(tektite_core STATIC
    "source/audio/audio.cpp"

    "source/engine/engine.cpp"

    "source/graphics/font.cpp"
    "source/graphics/graphics.cpp"
    "source/graphics/texture.cpp"

    "source/input/input.cpp"

    "source/script/module/audio_module.cpp"
    "source/script/module/graphics_module.cpp"
    "source/script/module/input_module.cpp"
    "source/script/module/world_module.cpp"

    "source/script/object/entity_object.cpp"
    "source/script/object/font_object.cpp"
    "source/script/object/texture_object.cpp"

    "source/script/script.cpp"

    "source/third_party/miniaudio.c"
    
    "source/world/entity.cpp"
    "source/world/tilemap.cpp"
    "source/world/world.cpp"
)

set(LIBS glm lua_static jsoncpp_static)

if(WIN32)
    set(LIBS ${LIBS} glad)
endif()


target_include_directories(tektite_core PUBLIC "include" PRIVATE "source")
target_link_libraries(tektite_core PUBLIC ${LIBS})
