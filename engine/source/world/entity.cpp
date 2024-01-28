#include <engine/world/entity.hpp>
#include <engine/world/world.hpp>

namespace tektite
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

    bool Entity::collide_check(const std::vector<Entity *> &entities, float x, float y)
    {
        bool collide = false;
        for (uint32_t i = 0; i < entities.size(); ++i) {
            Entity *entity = entities[i];
            if (this == entity)
                continue;
            if ((m_x + x) < entity->get_x() + entity->get_width() && (m_x + x) + m_width > entity->get_x() &&
                (m_y + y) < entity->get_y() + entity->get_height() && (m_y + y) + m_height > entity->get_y())
                collide = true;
        }
        return collide;
    }

    Entity *Entity::collide_first(const std::vector<Entity *> &entities, float x, float y)
    {
        for (uint32_t i = 0; i < entities.size(); ++i) {
            Entity *entity = entities[i];
            if (this == entity)
                continue;
            if ((m_x + x) < entity->get_x() + entity->get_width() && (m_x + x) + m_width > entity->get_x() &&
                (m_y + y) < entity->get_y() + entity->get_height() && (m_y + y) + m_height > entity->get_y())
                return entity;
        }
        return nullptr;
    }

} // namespace tektite
