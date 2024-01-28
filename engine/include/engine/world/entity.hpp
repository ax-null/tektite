#pragma once

#include "world.hpp"

#include <engine/script/script_engine.hpp>

#include <stdint.h>
#include <string>
#include <vector>

namespace tektite
{
    class Entity
    {
      public:
        Entity(uint32_t tag, const char *name, float x, float y, float width, float height, World *world);
        virtual ~Entity() = default;

        virtual void update() = 0;
        virtual void render() = 0;

        virtual void destroy() = 0;

        bool collide_check(float x, float y) { return collide_check(m_world->get_entities(), x, y); }
        bool collide_check(uint32_t tag, float x, float y) { return collide_check(m_world->get_entities_for_tag(tag), x, y); }
        bool collide_check(const std::vector<Entity *> &entities, float x, float y);

        Entity *collide_first(float x, float y) { return collide_first(m_world->get_entities(), x, y); }
        Entity *collide_first(uint32_t tag, float x, float y) { return collide_first(m_world->get_entities_for_tag(tag), x, y); }
        Entity *collide_first(const std::vector<Entity *> &entities, float x, float y);

        void set_position(float x, float y)
        {
            m_x = x;
            m_y = y;
        }

        void set_size(float width, float height)
        {
            m_width = width;
            m_height = height;
        }

        uint32_t get_tag() const { return m_tag; }

        const char *get_name() const { return m_name.c_str(); }

        float get_x() const { return m_x; }
        float get_y() const { return m_y; }
        float get_width() const { return m_width; }
        float get_height() const { return m_height; }

      private:
        World *m_world = nullptr;

        uint32_t m_tag;
        std::string m_name;
        float m_x, m_y, m_width, m_height;
    };

} // namespace tektite
