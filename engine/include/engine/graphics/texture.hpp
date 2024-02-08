#pragma once

#include <cstdint>

namespace paper
{

    class Texture
    {
      public:
        Texture(const char *path);
        ~Texture();

        uint32_t get_handle() const { return m_handle; }
        uint32_t get_width() const { return m_width; }
        uint32_t get_height() const { return m_height; }

      private:
        uint32_t m_handle, m_width, m_height;
    };

} // namespace paper
