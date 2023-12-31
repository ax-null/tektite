#pragma once

#include "core/graphics/graphics.hpp"
#include "core/script/script.hpp"
#include "core/world/tilemap.hpp"

#include <vector>

namespace Tektite
{
    class Entity;
    class World
    {
      public:
        World(Script *script) : m_script(script) {}
        ~World() = default;

        void update();
        void render(Graphics *graphics);

        void loadMap(const char *tilemapPath);

        void addEntity(Entity *entity);

        const std::vector<Entity *> &getEntities() const { return m_entities; }
        const std::vector<Entity *> &getEntitiesForTag(uint32_t tag);

        const TileMap *getTileMap() const { return m_tilemap.get(); }

      private:
        std::unique_ptr<TileMap> m_tilemap;
        std::vector<Entity *> m_entities, m_tagged;

        Script *m_script;
    };

} // namespace Tektite
