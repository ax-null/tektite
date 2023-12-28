#include <core/graphics/graphics.hpp>

#ifdef __EMSCRIPTEN__
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <GLES2/gl2platform.h>
#else
#include <glad/gl.h>
#endif

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

const uint32_t max_item_count = 1000;
const uint32_t max_vertex_count = max_item_count * 8;
const uint32_t max_index_count = max_item_count;

typedef struct
{
    GLuint program, default_texture, texture, vao, vbo, ebo, mode;
    uint32_t *indices, vertex_count, index_count, vertex_index;
    float *vertices, red, green, blue, alpha;
    glm::mat4 matrix;
} renderer_state_t;

static renderer_state_t state = {};

const char *vertex_shader_source =
    "attribute vec2 aPos;\n"
    "attribute vec2 aTexCoord;\n"
    "attribute vec4 aColor;\n"
    "varying vec2 vTexCoord;\n"
    "varying vec4 vColor;\n"
    "uniform mat4 uMatrix;\n"
    "void main()\n"
    "{\n"
    "   vTexCoord = aTexCoord;\n"
    "   vColor = aColor;\n"
    "   gl_Position = uMatrix * vec4(aPos, 0.0, 1.0);\n"
    "}\0";

const char *fragment_shader_source =
    "precision mediump float;\n"
    "varying vec2 vTexCoord;\n"
    "varying vec4 vColor;\n"
    "uniform sampler2D uTexture;\n"
    "void main()\n"
    "{\n"
    "   gl_FragColor = texture2D(uTexture, vTexCoord) * vColor;\n"
    "}\0";

#define VERTEX(x, y, u, v)                              \
    state.vertices[state.vertex_count++] = x;           \
    state.vertices[state.vertex_count++] = y;           \
    state.vertices[state.vertex_count++] = u;           \
    state.vertices[state.vertex_count++] = v;           \
    state.vertices[state.vertex_count++] = state.red;   \
    state.vertices[state.vertex_count++] = state.green; \
    state.vertices[state.vertex_count++] = state.blue;  \
    state.vertices[state.vertex_count++] = state.alpha; \
    state.vertex_index++;

#define INDEX_6(i0, i1, i2, i3, i4, i5)                           \
    state.indices[state.index_count++] = i0 + state.vertex_index; \
    state.indices[state.index_count++] = i1 + state.vertex_index; \
    state.indices[state.index_count++] = i2 + state.vertex_index; \
    state.indices[state.index_count++] = i3 + state.vertex_index; \
    state.indices[state.index_count++] = i4 + state.vertex_index; \
    state.indices[state.index_count++] = i5 + state.vertex_index;

#define CHECK(gl_mode, gl_texture, vertex_count, index_count)                                                                       \
    if (state.mode != gl_mode || state.texture != gl_texture || max_vertex_count >= vertex_count || max_index_count >= index_count) \
        submit();                                                                                                                   \
    state.mode = gl_mode;                                                                                                           \
    state.texture = gl_texture;

namespace Tektite
{

    Graphics::Graphics(uint32_t width, uint32_t height)
    {
        state.vertices = (float *)malloc(max_vertex_count * sizeof(float));
        state.indices = (uint32_t *)malloc(max_index_count * sizeof(uint32_t));

        glGenBuffers(1, &state.vbo);
        glBindBuffer(GL_ARRAY_BUFFER, state.vbo);
        glBufferData(GL_ARRAY_BUFFER, max_vertex_count * sizeof(float), state.vertices, GL_DYNAMIC_DRAW);

        glGenBuffers(1, &state.ebo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, state.ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, max_index_count * sizeof(uint32_t), state.indices, GL_DYNAMIC_DRAW);

        uint32_t vertex_shader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex_shader, 1, &vertex_shader_source, NULL);
        glCompileShader(vertex_shader);

        uint32_t fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment_shader, 1, &fragment_shader_source, NULL);
        glCompileShader(fragment_shader);

        state.program = glCreateProgram();
        glAttachShader(state.program, vertex_shader);
        glAttachShader(state.program, fragment_shader);

        glLinkProgram(state.program);

        glDeleteShader(vertex_shader);
        glDeleteShader(fragment_shader);

        uint32_t position = glGetAttribLocation(state.program, "aPos");
        uint32_t texture = glGetAttribLocation(state.program, "aTexCoord");
        uint32_t color = glGetAttribLocation(state.program, "aColor");

        glEnableVertexAttribArray(position);
        glVertexAttribPointer(position, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);

        glEnableVertexAttribArray(texture);
        glVertexAttribPointer(texture, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(2 * sizeof(float)));

        glEnableVertexAttribArray(color);
        glVertexAttribPointer(color, 4, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(4 * sizeof(float)));

        glGenTextures(1, &state.default_texture);
        glBindTexture(GL_TEXTURE_2D, state.default_texture);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        uint8_t data[4] = { 0xff, 0xff, 0xff, 0xff };

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        state.matrix = glm::ortho<float>(0.0f, width, 0.0f, height, -1.0f, 1.0f);
    }

    Graphics::~Graphics()
    {
        free(state.vertices);
        free(state.indices);

        glDeleteTextures(1, &state.default_texture);

        glDeleteBuffers(1, &state.vbo);
        glDeleteBuffers(1, &state.ebo);

        glDeleteProgram(state.program);
    }

    void Graphics::clear(float red, float green, float blue, float alpha)
    {
        glClearColor(red, green, blue, alpha);
        glClear(GL_COLOR_BUFFER_BIT);
    }

    void Graphics::submit()
    {
        glBindBuffer(GL_ARRAY_BUFFER, state.vbo);
        glBufferData(GL_ARRAY_BUFFER, state.vertex_count * sizeof(float), state.vertices, GL_DYNAMIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, state.ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, state.index_count * sizeof(uint32_t), state.indices, GL_DYNAMIC_DRAW);

        glBindTexture(GL_TEXTURE_2D, state.texture);

        glUseProgram(state.program);

        uint32_t location = glGetUniformLocation(state.program, "uMatrix");
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(state.matrix));

        glDrawElements(state.mode, state.index_count, GL_UNSIGNED_INT, 0);

        state.vertex_count = state.index_count = state.vertex_index = 0;
    }

    void Graphics::setColor(float red, float green, float blue, float alpha)
    {
        state.red = red;
        state.green = green;
        state.blue = blue;
        state.alpha = alpha;
    }

    void Graphics::drawPoint(float x, float y)
    {
        CHECK(GL_POINTS, state.default_texture, 1, 1);
        state.indices[state.index_count++] = 0 + state.vertex_index;
        VERTEX(x, y, 0, 0);
    }

    void Graphics::drawLine(float x1, float y1, float x2, float y2)
    {
        CHECK(GL_LINES, state.default_texture, 2, 2);
        state.indices[state.index_count++] = 0 + state.vertex_index;
        state.indices[state.index_count++] = 1 + state.vertex_index;
        VERTEX(x1, y1, 0, 0);
        VERTEX(x2, y2, 0, 0);
    }

    void Graphics::drawTriangle(float x1, float y1, float x2, float y2, float x3, float y3, bool fill)
    {
        if (fill) {
            CHECK(GL_TRIANGLES, state.default_texture, 3, 3);
            state.indices[state.index_count++] = 0 + state.vertex_index;
            state.indices[state.index_count++] = 1 + state.vertex_index;
            state.indices[state.index_count++] = 2 + state.vertex_index;
            VERTEX(x1, y1, 0, 0);
            VERTEX(x2, y2, 0, 0);
            VERTEX(x3, y3, 0, 0);
        } else {
            CHECK(GL_LINES, state.default_texture, 6, 6);
            INDEX_6(0, 1, 2, 3, 4, 5);
            VERTEX(x1, y1, 0, 0);
            VERTEX(x2, y2, 0, 0);
            VERTEX(x2, y2, 0, 0);
            VERTEX(x3, y3, 0, 0);
            VERTEX(x3, y3, 0, 0);
            VERTEX(x1, y1, 0, 0);
        }
    }

    void Graphics::drawRect(float x, float y, float width, float height, bool fill)
    {
        if (fill) {
            CHECK(GL_TRIANGLES, state.default_texture, 4, 6);
            INDEX_6(0, 1, 2, 2, 3, 0);
            VERTEX(x + width, y, 0, 0);
            VERTEX(x + width, y + height, 0, 0);
            VERTEX(x, y + height, 0, 0);
            VERTEX(x, y, 0, 0);
        } else {
            CHECK(GL_LINES, state.default_texture, 8, 8);
            INDEX_6(0, 1, 2, 3, 4, 5);
            state.indices[state.index_count++] = 6 + state.vertex_index;
            state.indices[state.index_count++] = 7 + state.vertex_index;
            VERTEX(x, y, 0, 0);
            VERTEX(x + width, y, 0, 0);
            VERTEX(x + width, y, 0, 0);
            VERTEX(x + width, y + height, 0, 0);
            VERTEX(x, y, 0, 0);
            VERTEX(x, y + height, 0, 0);
            VERTEX(x, y + height, 0, 0);
            VERTEX(x + width, y + height, 0, 0);
        }
    }

    void Graphics::drawCircle(float x, float y, float radius, bool fill)
    {
        float amount = glm::max<float>(1, (int)(6 * (float)cbrtf(radius)));

        if (fill) {
            CHECK(GL_TRIANGLE_FAN, state.default_texture, amount + 1, amount + 1);
        } else {
            CHECK(GL_LINE_STRIP, state.default_texture, amount, amount);
        }

        for (int32_t i = 0; i <= amount; i++)
            state.indices[state.index_count++] = i + state.vertex_index;

        if (fill) {
            state.indices[state.index_count++] = (amount + 1) + state.vertex_index;
            VERTEX(x, y, 0, 0);
        }

        for (int32_t i = 0; i <= amount; i++) {
            VERTEX(
                x + (float)(radius * cosf(i * (glm::pi<float>() * 2.0f) / amount)),
                y + (float)(radius * sinf(i * (glm::pi<float>() * 2.0f) / amount)), 0, 0);
        }
    }

    void Graphics::drawTexture(Texture *texture, float x, float y, float width, float height,
                               float sourceX, float sourceY, float sourceWidth, float sourceHeight,
                               float rotation, bool flipX, bool flipY)
    {
        CHECK(GL_TRIANGLES, texture->getHandle(), 4, 6);

        if (sourceWidth == -1.0f)
            sourceWidth = texture->getWidth();
        if (sourceHeight == -1.0f)
            sourceHeight = texture->getHeight();

        if (width == -1.0f)
            width = texture->getWidth();
        if (height == -1.0f)
            height = texture->getHeight();

        float u = sourceX * (1.0f / texture->getWidth());
        float v = (sourceY + sourceHeight) * (1.0f / texture->getHeight());
        float u2 = (sourceX + sourceWidth) * (1.0f / texture->getWidth());
        float v2 = sourceY * (1.0f / texture->getHeight());

        if (flipX) {
            u = 1.0f - u;
            u2 = 1.0f - u2;
        }

        if (flipY) {
            v = 1.0f - v;
            v2 = 1.0f - v2;
        }

        INDEX_6(0, 1, 2, 2, 3, 0);
        VERTEX(x + width, y, u2, v);
        VERTEX(x + width, y + height, u2, v2);
        VERTEX(x, y + height, u, v2);
        VERTEX(x, y, u, v);
    }

    void Graphics::drawString(Font *font, const char *text, float x, float y)
    {
        int last = 0, length = strlen(text);
        float xoffset = x, yoffset = y;

        for (int i = 0; i < length; ++i) {
            if (text[i] == '\n') {
                yoffset -= font->getLineHeight();
                xoffset = x;
            }

            const Font::Glyph &glyph = font->getGlyph(text[i]);
            const Texture *texture = font->getTexture(glyph.textureIndex);

            CHECK(GL_TRIANGLES, texture->getHandle(), 4, 6);

            float u = glyph.x * (1.0f / texture->getWidth());
            float v = (glyph.y + glyph.height) * (1.0f / texture->getHeight());
            float u2 = (glyph.x + glyph.width) * (1.0f / texture->getWidth());
            float v2 = glyph.y * (1.0f / texture->getHeight());

            if (i > 0 && text[i - 1] != '\n')
                xoffset += font->getKerning(last, text[i]);

            INDEX_6(0, 1, 2, 2, 3, 0);
            VERTEX(xoffset + glyph.width, y, u2, v);
            VERTEX(xoffset + glyph.width, y + glyph.height, u2, v2);
            VERTEX(xoffset, yoffset + glyph.height, u, v2);
            VERTEX(xoffset, yoffset, u, v);

            xoffset += glyph.advance;
            last = text[i];
        }
    }

} // namespace Tektite
