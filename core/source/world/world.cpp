#include <core/script/script.hpp>
#include <core/world/entity.hpp>
#include <core/world/world.hpp>

#include <cstdint>

namespace Tektite
{
    void World::update()
    {
        for (uint32_t i = 0; i < m_entities.size(); ++i)
            m_entities[i]->update();
    }

    void World::render(Graphics *graphics)
    {
        graphics->setColor(1.0f, 1.0f, 1.0f, 1.0f);
        graphics->drawTiles(m_tilemap->getTileSet(), m_tilemap->getBackground().data,
                            m_tilemap->getTileWidth(), m_tilemap->getTileHeight(),
                            m_tilemap->getBackground().width, m_tilemap->getBackground().height, 0, 0);

        graphics->drawTiles(m_tilemap->getTileSet(), m_tilemap->getForeground().data,
                            m_tilemap->getTileWidth(), m_tilemap->getTileHeight(),
                            m_tilemap->getForeground().width, m_tilemap->getForeground().height, 0, 0);

        for (uint32_t i = 0; i < m_entities.size(); ++i)
            m_entities[i]->render();
    }

    void World::loadMap(const char *tileMapPath)
    {
        m_tilemap = std::make_unique<TileMap>(tileMapPath);

        float yDown = m_tilemap->getTileHeight() * m_tilemap->getSizeY();

        for (EntityData entity : m_tilemap->getEntities()) {
            lua_pushnumber(m_script->getState(), entity.tag);
            lua_pushstring(m_script->getState(), entity.name.c_str());
            lua_pushnumber(m_script->getState(), entity.x);
            lua_pushnumber(m_script->getState(), yDown - (entity.y + entity.height));
            lua_pushnumber(m_script->getState(), entity.width);
            lua_pushnumber(m_script->getState(), entity.height);
            lua_pushlightuserdata(m_script->getState(), this);
            m_script->execute(entity.scriptPath.c_str());
        }
    }

    void World::addEntity(Entity *entity)
    {
        m_entities.push_back(entity);
    }

    const std::vector<Entity *> &World::getEntitiesForTag(uint32_t tag)
    {
        m_tagged.clear();
        for (uint32_t i = 0; i < m_entities.size(); ++i) {
            if ((m_entities[i]->getTag() & tag) == tag)
                m_tagged.push_back(m_entities[i]);
        }
        return m_tagged;
    }

} // namespace Tektite
