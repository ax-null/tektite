#pragma once

#include "world/tilemap.hpp"

#include "graphics.hpp"
#include "script.hpp"

#include <vector>

namespace paper
{

    class Entity;

    class World
    {
      public:
        World(Script *script) : m_script_engine(script) {}
        ~World() = default;

        void update();
        void render(Graphics *graphics);

        void load_map(const char *tilemapPath);

        void add_entity(Entity *entity);

        const std::vector<Entity *> &get_entities() const { return m_entities; }
        const std::vector<Entity *> &get_entities_for_tag(uint32_t tag);

        const TileMap *get_tilemap() const { return m_tilemap.get(); }

      private:
        std::unique_ptr<TileMap> m_tilemap;
        std::vector<Entity *> m_entities, m_tagged;

        Script *m_script_engine;
    };

} // namespace paper
