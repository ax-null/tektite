#include <core/world/entity.hpp>
#include <core/world/world.hpp>

namespace Tektite
{

    Entity::Entity(uint32_t tag, const char *name, float x, float y, float width, float height, World *world)
    {
        m_tag = tag;
        m_name = name;
        m_x = x;
        m_y = y;
        m_width = width;
        m_height = height;
        m_world = world;
    }

    bool Entity::collideCheck(const std::vector<Entity *> &entities, float x, float y)
    {
        bool collide = false;
        for (uint32_t i = 0; i < entities.size(); ++i) {
            Entity *entity = entities[i];
            if (this == entity)
                continue;
            if ((m_x + x) < entity->getX() + entity->getWidth() && (m_x + x) + m_width > entity->getX() &&
                (m_y + y) < entity->getY() + entity->getHeight() && (m_y + y) + m_height > entity->getY())
                collide = true;
        }
        return collide;
    }

    Entity *Entity::collideFirst(const std::vector<Entity *> &entities, float x, float y)
    {
        for (uint32_t i = 0; i < entities.size(); ++i) {
            Entity *entity = entities[i];
            if (this == entity)
                continue;
            if ((m_x + x) < entity->getX() + entity->getWidth() && (m_x + x) + m_width > entity->getX() &&
                (m_y + y) < entity->getY() + entity->getHeight() && (m_y + y) + m_height > entity->getY())
                return entity;
        }
        return nullptr;
    }

} // namespace Tektite
