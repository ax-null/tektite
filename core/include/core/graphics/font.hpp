#pragma once

#include "texture.hpp"

#include <cstdint>
#include <string>
#include <unordered_map>

namespace Tektite
{
    class Font
    {
      public:
        struct Glyph
        {
            uint32_t id, textureIndex, channel;
            float x, y, width, height, advance, offsetX, offsetY;
        };

        Font(const char *path);
        ~Font();

        const Glyph &operator[](uint32_t codepoint) const { return getGlyph(codepoint); }

        const Glyph &getGlyph(int32_t codepoint) const;
        float getKerning(int32_t first, int32_t second) const;

        const std::string &getName() const { return m_name; }

        const Texture *getTexture(uint32_t index) const { return m_textures[index]; }

        uint32_t getSize() const { return m_size; }
        uint32_t getLineHeight() const { return m_lineHeight; }

      private:
        std::string m_name;

        uint32_t m_size, m_lineHeight;

        std::unordered_map<int32_t, Glyph> m_glyphs;
        std::unordered_map<int64_t, float> m_kernings;

        Texture **m_textures;
    };
} // namespace Tektite
