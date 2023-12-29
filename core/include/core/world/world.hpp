#pragma once

#include "core/graphics/graphics.hpp"
#include "core/script/script.hpp"
#include "core/world/tilemap.hpp"
#include "entity.hpp"
#include <vector>

namespace Tektite
{
    class World
    {
      public:
        World(Script *script);
        ~World() = default;

        void update();
        void render(Graphics *graphics);

        void loadMap(const char *tilemapPath);

        void addEntity(Entity *entity);

      private:
        std::unique_ptr<TileMap> m_tilemap;
        std::vector<Entity *> m_entities;

        Script *m_script;
    };

} // namespace Tektite
