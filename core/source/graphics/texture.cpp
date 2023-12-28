#include <core/graphics/texture.hpp>

#ifdef __EMSCRIPTEN__
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <GLES2/gl2platform.h>
#else
#include <glad/gl.h>
#endif

#define STB_IMAGE_IMPLEMENTATION
#include "../third_party/stb_image.h"

namespace Tektite
{

    Texture::Texture(const char *path)
    {
        glGenTextures(1, &m_handle);
        glBindTexture(GL_TEXTURE_2D, m_handle);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        int32_t width;
        int32_t height;
        int32_t channels;

        uint8_t *data = stbi_load(path, &width, &height, &channels, 4);

        if (data) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }

        m_width = width;
        m_height = height;
    }
    Texture::~Texture()
    {
        glDeleteTextures(1, &m_handle);
    }

} // namespace Tektite
