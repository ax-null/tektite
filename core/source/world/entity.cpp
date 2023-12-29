#include <core/world/entity.hpp>

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

} // namespace Tektite
