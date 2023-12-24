add_library(tektite_core STATIC
	"source/third_party/miniaudio.c"
    "source/engine.cpp"
)

set(LIBS glm lua_static jsoncpp_static)

if(WIN32)
    set(LIBS ${LIBS} glad)
endif()

target_include_directories(tektite_core PUBLIC "include" PRIVATE "source")
target_link_libraries(tektite_core PUBLIC ${LIBS})
