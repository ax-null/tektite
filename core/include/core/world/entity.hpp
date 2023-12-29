#pragma once

#include <stdint.h>
#include <string>

namespace Tektite
{
    class World;
    class Entity
    {
      public:
        Entity(uint32_t tag, const char *name, float x, float y, float width, float height, World *world);
        virtual ~Entity() = default;

        virtual void update() = 0;
        virtual void render() = 0;

        virtual void destroy() = 0;

        void setPosition(float x, float y)
        {
            m_x = x;
            m_y = y;
        }

        void setSize(float width, float height)
        {
            m_width = width;
            m_height = height;
        }

        uint32_t getTag() const { return m_tag; }

        const char *getName() const { return m_name.c_str(); }

        float getX() const { return m_x; }
        float getY() const { return m_y; }
        float getWidth() const { return m_width; }
        float getHeight() const { return m_height; }

      private:
        World *m_world = nullptr;

        uint32_t m_tag;
        std::string m_name;
        float m_x, m_y, m_width, m_height;
    };

} // namespace Tektite
