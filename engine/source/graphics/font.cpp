#include <engine/graphics/font.hpp>
#include <fstream>

namespace tektite
{
    Font::Font(const char *path)
    {
        std::string name;
        std::string *pages;

        int32_t size, line_height, page_count;

        std::unordered_map<int32_t, Font::Glyph> glyphs;
        std::unordered_map<int64_t, float> kernings;

        std::ifstream file(path);
        std::string line;

        while (std::getline(file, line)) {
            if (line[0] == 'i' && line[1] == 'n') {
                char value[512];
                sscanf(line.c_str(), "info face=\"%[^\"]\" size=%d", value, &size);
                name = value;
            } else if (line[0] == 'c' && line[1] == 'o') {
                sscanf(line.c_str(), "common lineHeight=%d %*[^p] pages=%d", &line_height, &page_count);
                pages = new std::string[page_count];
            } else if (line[0] == 'p' && line[1] == 'a') {
                int id = 0;
                char file[512];
                sscanf(line.c_str(), "page id=%d file=\"%[^\"]\"", &id, file);
                pages[id] = file;
            } else if (line[0] == 'c' && line[1] == 'h' && line[4] == ' ') {
                Font::Glyph glyph = {};
                sscanf(
                    line.c_str(),
                    "char id=%d%*[ ]x=%f%*[ ]y=%f%*[ ]width=%f%*[ ]height=%f%*[ ]xoffset=%f%*[ ]yoffset=%f%*[ ]xadvance=%f%*[ ]page=%d",
                    &glyph.id,
                    &glyph.x,
                    &glyph.y,
                    &glyph.width,
                    &glyph.height,
                    &glyph.offsetX,
                    &glyph.offsetY,
                    &glyph.advance,
                    &glyph.texture_index);
                glyphs[glyph.id] = glyph;
            } else if (line[0] == 'k' && line[1] == 'e' && line[7] == ' ') {
                int first, second;
                float amount;
                sscanf(line.c_str(), "kerning first=%d second=%d amount=%f", &first, &second, &amount);
                kernings[((uint64_t)first << 32) | second] = amount;
            }
        }

        Texture **textures = new Texture *[page_count];

        for (uint32_t i = 0; i < page_count; ++i)
            textures[i] = new Texture(pages[i].c_str());

        m_name = name;
        m_size = size;
        m_line_height = line_height;
        m_glyphs = glyphs;
        m_kernings = kernings;
        m_textures = textures;
    }

    Font::~Font()
    {
        m_glyphs.clear();
        m_kernings.clear();

        delete[] m_textures;
    }

    const Font::Glyph &Font::get_glyph(int32_t codepoint) const
    {
        auto glyph = m_glyphs.find(codepoint);
        return glyph->second;
    }

    float Font::get_kerning(int32_t first, int32_t second) const
    {
        auto kerning = m_kernings.find(((int64_t)first << 32) | second);
        if (kerning == m_kernings.end())
            return 0;
        else
            return kerning->second;
    }

} // namespace tektite
