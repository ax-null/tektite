#include <core/graphics/graphics.hpp>
#include <memory>

#ifdef __EMSCRIPTEN__
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <GLES2/gl2platform.h>
#else
#include <glad/gl.h>
#endif

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define VERTEX(x, y, u, v)                  \
    m_vertices[m_vertex_count++] = x;       \
    m_vertices[m_vertex_count++] = y;       \
    m_vertices[m_vertex_count++] = u;       \
    m_vertices[m_vertex_count++] = v;       \
    m_vertices[m_vertex_count++] = m_red;   \
    m_vertices[m_vertex_count++] = m_green; \
    m_vertices[m_vertex_count++] = m_blue;  \
    m_vertices[m_vertex_count++] = m_alpha; \
    m_vertex_index++;

#define INDEX_6(i0, i1, i2, i3, i4, i5)               \
    m_indices[m_index_count++] = i0 + m_vertex_index; \
    m_indices[m_index_count++] = i1 + m_vertex_index; \
    m_indices[m_index_count++] = i2 + m_vertex_index; \
    m_indices[m_index_count++] = i3 + m_vertex_index; \
    m_indices[m_index_count++] = i4 + m_vertex_index; \
    m_indices[m_index_count++] = i5 + m_vertex_index;

#define CHECK(gl_mode, gl_texture, gl_vertex_count, gl_index_count) \
    if (m_mode != gl_mode || m_texture != gl_texture ||             \
        m_vertex_count + gl_vertex_count >= VERTEX_COUNT ||         \
        m_index_count + gl_index_count >= INDEX_COUNT)              \
        submit();                                                   \
    m_mode = gl_mode;                                               \
    m_texture = gl_texture;

namespace Tektite
{

    Graphics::Graphics(uint32_t width, uint32_t height)
    {
        m_camera = std::make_unique<Camera>();
        m_virtualWidth = width;
        m_virtualHeight = height;

        glGenBuffers(1, &m_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(m_vertices), m_vertices, GL_DYNAMIC_DRAW);

        glGenBuffers(1, &m_ebo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(m_indices), m_indices, GL_DYNAMIC_DRAW);

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

        uint32_t vertex_shader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex_shader, 1, &vertex_shader_source, NULL);
        glCompileShader(vertex_shader);

        uint32_t fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment_shader, 1, &fragment_shader_source, NULL);
        glCompileShader(fragment_shader);

        m_program = glCreateProgram();
        glAttachShader(m_program, vertex_shader);
        glAttachShader(m_program, fragment_shader);

        glLinkProgram(m_program);

        glDeleteShader(vertex_shader);
        glDeleteShader(fragment_shader);

        uint32_t position = glGetAttribLocation(m_program, "aPos");
        uint32_t texture = glGetAttribLocation(m_program, "aTexCoord");
        uint32_t color = glGetAttribLocation(m_program, "aColor");

        glEnableVertexAttribArray(position);
        glVertexAttribPointer(position, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);

        glEnableVertexAttribArray(texture);
        glVertexAttribPointer(texture, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(2 * sizeof(float)));

        glEnableVertexAttribArray(color);
        glVertexAttribPointer(color, 4, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(4 * sizeof(float)));

        glGenTextures(1, &m_tex);
        glBindTexture(GL_TEXTURE_2D, m_tex);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        uint8_t data[4] = { 0xff, 0xff, 0xff, 0xff };

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }

    Graphics::~Graphics()
    {
        glDeleteTextures(1, &m_tex);

        glDeleteBuffers(1, &m_vbo);
        glDeleteBuffers(1, &m_ebo);

        glDeleteProgram(m_program);
    }

    void Graphics::clear(float red, float green, float blue, float alpha)
    {
        glClearColor(red, green, blue, alpha);
        glViewport(m_viewportX, m_viewportY, m_viewportWidth, m_viewportHeight);
        glClear(GL_COLOR_BUFFER_BIT);
    }

    void Graphics::resize(uint32_t width, uint32_t height)
    {
        m_screenWidth = width;
        m_screenHeight = height;
        float targetRatio = m_screenHeight / m_screenWidth;
        float sourceRatio = m_virtualHeight / m_virtualWidth;
        float scale = targetRatio > sourceRatio ? m_screenWidth / m_virtualWidth : m_screenHeight / m_virtualHeight;
        int viewportWidth = glm::roundEven(m_virtualWidth * scale);
        int viewportHeight = glm::roundEven(m_virtualHeight * scale);
        m_viewportX = (m_screenWidth - viewportWidth) / 2.0f;
        m_viewportY = (m_screenHeight - viewportHeight) / 2.0f;
        m_viewportWidth = viewportWidth;
        m_viewportHeight = viewportHeight;
        m_projection = glm::ortho<float>(0.0f, m_virtualWidth, 0.0f, m_virtualHeight, 0.0f, 1.0f);
    }

    void Graphics::submit()
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(m_vertices), m_vertices, GL_DYNAMIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(m_indices), m_indices, GL_DYNAMIC_DRAW);

        glBindTexture(GL_TEXTURE_2D, m_texture);

        glUseProgram(m_program);

        uint32_t location = glGetUniformLocation(m_program, "uMatrix");
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(m_projection * m_camera->getMatrix()));

        glDrawElements(m_mode, m_index_count, GL_UNSIGNED_INT, 0);

        m_vertex_count = m_index_count = m_vertex_index = 0;
        drawCalls++;
    }

    void Graphics::setColor(float red, float green, float blue, float alpha)
    {
        m_red = red;
        m_green = green;
        m_blue = blue;
        m_alpha = alpha;
    }

    void Graphics::drawPoint(float x, float y)
    {
        CHECK(GL_POINTS, m_tex, 1, 1);
        m_indices[m_index_count++] = 0 + m_vertex_index;
        VERTEX(x, y, 0, 0);
    }

    void Graphics::drawLine(float x1, float y1, float x2, float y2)
    {
        CHECK(GL_LINES, m_tex, 2, 2);
        m_indices[m_index_count++] = 0 + m_vertex_index;
        m_indices[m_index_count++] = 1 + m_vertex_index;
        VERTEX(x1, y1, 0, 0);
        VERTEX(x2, y2, 0, 0);
    }

    void Graphics::drawTriangle(float x1, float y1, float x2, float y2, float x3, float y3, bool fill)
    {
        if (fill) {
            CHECK(GL_TRIANGLES, m_tex, 3, 3);
            m_indices[m_index_count++] = 0 + m_vertex_index;
            m_indices[m_index_count++] = 1 + m_vertex_index;
            m_indices[m_index_count++] = 2 + m_vertex_index;
            VERTEX(x1, y1, 0, 0);
            VERTEX(x2, y2, 0, 0);
            VERTEX(x3, y3, 0, 0);
        } else {
            CHECK(GL_LINES, m_tex, 6, 6);
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
            CHECK(GL_TRIANGLES, m_tex, 4, 6);
            INDEX_6(0, 1, 2, 2, 3, 0);
            VERTEX(x + width, y, 0, 0);
            VERTEX(x + width, y + height, 0, 0);
            VERTEX(x, y + height, 0, 0);
            VERTEX(x, y, 0, 0);
        } else {
            CHECK(GL_LINES, m_tex, 8, 8);
            INDEX_6(0, 1, 2, 3, 4, 5);
            m_indices[m_index_count++] = 6 + m_vertex_index;
            m_indices[m_index_count++] = 7 + m_vertex_index;
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
            CHECK(GL_TRIANGLE_FAN, m_tex, amount + 1, amount + 1);
        } else {
            CHECK(GL_LINE_STRIP, m_tex, amount, amount);
        }

        for (int32_t i = 0; i <= amount; i++)
            m_indices[m_index_count++] = i + m_vertex_index;

        if (fill) {
            m_indices[m_index_count++] = (amount + 1) + m_vertex_index;
            VERTEX(x, y, 0, 0);
        }

        for (int32_t i = 0; i <= amount; i++) {
            VERTEX(
                x + (float)(radius * cosf(i * (glm::pi<float>() * 2.0f) / amount)),
                y + (float)(radius * sinf(i * (glm::pi<float>() * 2.0f) / amount)), 0, 0);
        }
    }

    void Graphics::drawTexture(const Texture *texture, float x, float y, float width, float height,
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

    void Graphics::drawString(const Font *font, const char *text, float x, float y)
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
            VERTEX(xoffset + glyph.width, yoffset, u2, v);
            VERTEX(xoffset + glyph.width, yoffset + glyph.height, u2, v2);
            VERTEX(xoffset, yoffset + glyph.height, u, v2);
            VERTEX(xoffset, yoffset, u, v);

            xoffset += glyph.advance;
            last = text[i];
        }
    }

    void Graphics::drawTiles(const Texture *texture, const uint32_t *indices, uint32_t tileWidth, uint32_t tileHeight, uint32_t sizeX, uint32_t sizeY, float x, float y)
    {
        uint32_t length = (sizeX * sizeY) - 1;

        for (int i = 0; i < sizeX * sizeY; ++i) {
            if (indices[i] == 0)
                continue;

            CHECK(GL_TRIANGLES, texture->getHandle(), 4, 6);

            uint32_t tilePosX = (i % sizeX) * tileWidth;
            uint32_t tilePosY = ((length - i) / sizeX) * tileHeight;

            uint32_t tileX = (indices[i] - 1) % (texture->getWidth() / tileWidth) * tileWidth;
            uint32_t tileY = (indices[i] - 1) / (texture->getWidth() / tileWidth) * tileHeight;

            float u = tileX * (1.0f / texture->getWidth());
            float v = (tileY + tileHeight) * (1.0f / texture->getHeight());
            float u2 = (tileX + tileWidth) * (1.0f / texture->getWidth());
            float v2 = tileY * (1.0f / texture->getHeight());

            INDEX_6(0, 1, 2, 2, 3, 0);
            VERTEX(tilePosX + tileWidth, tilePosY, u2, v);
            VERTEX(tilePosX + tileWidth, tilePosY + tileHeight, u2, v2);
            VERTEX(tilePosX, tilePosY + tileHeight, u, v2);
            VERTEX(tilePosX, tilePosY, u, v);
        }
    }

} // namespace Tektite
