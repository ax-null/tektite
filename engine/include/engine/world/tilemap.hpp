#pragma once

#include <engine/graphics/texture.hpp>

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

namespace tektite
{
    struct EntityData
    {
        uint32_t tag;
        float x, y, width, height;
        std::string name;
        std::string script_path;
    };

    struct TileLayer
    {
        uint32_t width, height, *data;
    };

    class TileMap
    {
      public:
        TileMap(const char *path);
        ~TileMap();

        uint32_t get_size_x() const { return m_size_x; }
        uint32_t get_size_y() const { return m_size_y; }

        uint32_t get_tile_width() const { return m_tile_width; }
        uint32_t get_tile_height() const { return m_tile_height; }

        const TileLayer &get_background() const { return m_background; }
        const TileLayer &get_foreground() const { return m_foreground; }

        const std::vector<EntityData> &get_entities() const { return m_entities; }

        const Texture *get_tileset() const { return m_tileset.get(); }

      private:
        uint32_t m_tile_width, m_tile_height, m_size_x, m_size_y;
        TileLayer m_background, m_foreground;

        std::vector<EntityData> m_entities;
        std::unique_ptr<Texture> m_tileset;
    };

} // namespace tektite
