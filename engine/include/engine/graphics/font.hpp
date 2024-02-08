#pragma once

#include "texture.hpp"

#include <cstdint>
#include <string>
#include <unordered_map>

namespace paper
{

    class Font
    {
      public:
        struct Glyph
        {
            uint32_t id, texture_index, channel;
            float x, y, width, height, advance, offsetX, offsetY;
        };

        Font(const char *path);
        ~Font();

        const Glyph &operator[](uint32_t codepoint) const { return get_glyph(codepoint); }

        const Glyph &get_glyph(int32_t codepoint) const;
        float get_kerning(int32_t first, int32_t second) const;

        const std::string &get_name() const { return m_name; }

        const Texture *get_texture(uint32_t index) const { return m_textures[index]; }

        uint32_t get_size() const { return m_size; }
        uint32_t get_line_height() const { return m_line_height; }

      private:
        std::string m_name;

        uint32_t m_size, m_line_height;

        std::unordered_map<int32_t, Glyph> m_glyphs;
        std::unordered_map<int64_t, float> m_kernings;

        Texture **m_textures;
    };

} // namespace paper
