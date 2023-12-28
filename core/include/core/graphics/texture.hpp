#pragma once

#include <cstdint>

namespace Tektite
{

    class Texture
    {
      public:
        Texture(const char *path);
        ~Texture();

        uint32_t getHandle() const { return m_handle; }
        uint32_t getWidth() const { return m_width; }
        uint32_t getHeight() const { return m_height; }

      private:
        uint32_t m_handle, m_width, m_height;
    };

} // namespace Tektite
