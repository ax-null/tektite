#include <core/engine.hpp>
#include <memory>

#ifdef __EMSCRIPTEN__
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <GLES2/gl2platform.h>
#else
#include <glad/gl.h>
#endif

extern "C" {
#include <lauxlib.h>
#include <lua.h>
#include <lualib.h>
}

#include <fstream>
#include <json/json.h>

#include "third_party/miniaudio.h"

// Shader sources
static const GLchar *vertexSource =
    "attribute vec2 position;                   \n"
    "void main()                                \n"
    "{                                          \n"
    "   gl_Position = vec4(position, 0.0, 1.0); \n"
    "}                            \n";
static const GLchar *fragmentSource =
    "precision mediump float;                   \n"
    "void main()                                \n"
    "{                                          \n"
    "  gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0); \n"
    "}                                          \n";

static GLuint vbo, shaderProgram, posAttrib;
static ma_engine engine;
static float color;

namespace Tektite
{
    Engine::Engine()
    {
        m_input = std::make_unique<Input>();
        m_running = true;

        // Create a Vertex Buffer Object and copy the vertex data to it
        glGenBuffers(1, &vbo);

        GLfloat vertices[] = { 0.0f, 0.5f, 0.5f, -0.5f, -0.5f, -0.5f };

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        // Create and compile the vertex shader
        GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertexSource, nullptr);
        glCompileShader(vertexShader);

        // Create and compile the fragment shader
        GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragmentSource, nullptr);
        glCompileShader(fragmentShader);

        // Link the vertex and fragment shader into a shader program
        shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        glLinkProgram(shaderProgram);
        glUseProgram(shaderProgram);

        // Specify the layout of the vertex data
        glBindAttribLocation(shaderProgram, 0, "position");
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(0);

        int status;
        lua_State *L = luaL_newstate();
        luaL_openlibs(L);

        status = luaL_dofile(L, "assets/test.lua");
        if (status) {
            fprintf(stderr, "Error: %s\n", lua_tostring(L, -1));
        }

        Json::Value root;
        Json::Reader reader;

        std::ifstream ifs;
        ifs.open("assets/message.json");

        reader.parse(ifs, root);

        printf("%s\n", root["content"].asCString());

        if (ma_engine_init(NULL, &engine) != MA_SUCCESS)
            printf("Couldn't initialize audio engine.");
    }

    Engine::~Engine()
    {
        ma_engine_uninit(&engine);

        glDisableVertexAttribArray(0);
        glDeleteProgram(shaderProgram);
        glDeleteBuffers(1, &vbo);
    }

    void Engine::update()
    {
        m_input->update();

        if (m_input->isPressed(Keyboard::Escape))
            m_running = false;

        if (m_input->isPressed(Keyboard::Space))
            ma_engine_play_sound(&engine, "assets/hit_hurt.wav", NULL);

        if (m_input->isPressed(Mouse::Left))
            ma_engine_play_sound(&engine, "assets/laser.wav", NULL);

        color = fmod(color + 0.005f, 1.0f);
    }

    void Engine::render()
    {
        // Clear the screen to black
        glClearColor(color, 0.25f, 0.25f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Draw a triangle from the 3 vertices
        glDrawArrays(GL_TRIANGLES, 0, 3);
    }
} // namespace Tektite
