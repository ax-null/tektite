#include <engine/script/script_engine.hpp>
#include <engine/world/entity.hpp>
#include <engine/world/world.hpp>

#include <cstdint>

namespace tektite
{
    void World::update()
    {
        for (uint32_t i = 0; i < m_entities.size(); ++i)
            m_entities[i]->update();
    }

    void World::render(Graphics *graphics)
    {
        graphics->set_color(1.0f, 1.0f, 1.0f, 1.0f);
        graphics->draw_tiles(m_tilemap->get_tileset(), m_tilemap->get_background().data,
                             m_tilemap->get_tile_width(), m_tilemap->get_tile_height(),
                             m_tilemap->get_background().width, m_tilemap->get_background().height, 0, 0);

        graphics->draw_tiles(m_tilemap->get_tileset(), m_tilemap->get_foreground().data,
                             m_tilemap->get_tile_width(), m_tilemap->get_tile_height(),
                             m_tilemap->get_foreground().width, m_tilemap->get_foreground().height, 0, 0);

        for (uint32_t i = 0; i < m_entities.size(); ++i)
            m_entities[i]->render();
    }

    void World::load_map(const char *tileMapPath)
    {
        m_tilemap = std::make_unique<TileMap>(tileMapPath);

        float yDown = m_tilemap->get_tile_height() * m_tilemap->get_size_y();

        for (EntityData entity : m_tilemap->get_entities()) {
            lua_pushnumber(m_script_engine->get_state(), entity.tag);
            lua_pushstring(m_script_engine->get_state(), entity.name.c_str());
            lua_pushnumber(m_script_engine->get_state(), entity.x);
            lua_pushnumber(m_script_engine->get_state(), yDown - (entity.y + entity.height));
            lua_pushnumber(m_script_engine->get_state(), entity.width);
            lua_pushnumber(m_script_engine->get_state(), entity.height);
            lua_pushlightuserdata(m_script_engine->get_state(), this);
            m_script_engine->execute(entity.script_path.c_str());
        }
    }

    void World::add_entity(Entity *entity)
    {
        m_entities.push_back(entity);
    }

    const std::vector<Entity *> &World::get_entities_for_tag(uint32_t tag)
    {
        m_tagged.clear();
        for (uint32_t i = 0; i < m_entities.size(); ++i) {
            if ((m_entities[i]->get_tag() & tag) == tag)
                m_tagged.push_back(m_entities[i]);
        }
        return m_tagged;
    }

} // namespace tektite
