#pragma once

#include "core/graphics/texture.hpp"

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

namespace Tektite
{
    struct EntityData
    {
        uint32_t tag;
        float x, y, width, height;
        std::string name;
        std::string scriptPath;
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

        uint32_t getSizeX() const { return m_sizeX; }
        uint32_t getSizeY() const { return m_sizeY; }

        uint32_t getTileWidth() const { return m_tileWidth; }
        uint32_t getTileHeight() const { return m_tileHeight; }

        const TileLayer &getBackground() const { return m_background; }
        const TileLayer &getForeground() const { return m_foreground; }

        const std::vector<EntityData> &getEntities() const { return m_entities; }

        const Texture *getTileSet() const { return m_tileset.get(); }

      private:
        uint32_t m_tileWidth, m_tileHeight, m_sizeX, m_sizeY;
        TileLayer m_background, m_foreground;

        std::vector<EntityData> m_entities;
        std::unique_ptr<Texture> m_tileset;
    };

} // namespace Tektite
