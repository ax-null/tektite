#pragma once

#include "tilemap.hpp"

#include <engine/graphics/graphics.hpp>
#include <engine/script/script_engine.hpp>

#include <vector>

namespace tektite
{
    class Entity;
    class World
    {
      public:
        World(ScriptEngine *scripter) : m_script_engine(scripter) {}
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

        ScriptEngine *m_script_engine;
    };

} // namespace tektite
