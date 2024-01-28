#include <engine/world/tilemap.hpp>

#include <fstream>
#include <json/json.h>

namespace tektite
{

    TileMap::TileMap(const char *path)
    {
        Json::Value root;

        {
            Json::Reader reader;

            std::ifstream ifs;
            ifs.open(path);

            reader.parse(ifs, root);
        }

        m_size_x = root["width"].asInt();
        m_size_y = root["height"].asInt();

        m_tile_width = root["tilewidth"].asInt();
        m_tile_height = root["tileheight"].asInt();

        m_tileset = std::make_unique<Texture>(root["tilesets"][0]["properties"][0]["value"].asCString());

        // Background layer
        {
            uint32_t width = root["layers"][0]["width"].asInt();
            uint32_t height = root["layers"][0]["height"].asInt();

            m_background = { width, height, new uint32_t[width * height] };

            uint32_t index = 0;
            for (auto el : root["layers"][0]["data"])
                m_background.data[index++] = el.asInt();
        }

        // Foreground layer
        {
            uint32_t width = root["layers"][1]["width"].asInt();
            uint32_t height = root["layers"][1]["height"].asInt();

            m_foreground = { width, height, new uint32_t[width * height] };

            uint32_t index = 0;
            for (auto el : root["layers"][1]["data"])
                m_foreground.data[index++] = el.asInt();
        }

        // Entity layer
        {
            uint32_t index = 0;
            for (auto el : root["layers"][2]["objects"]) {
                EntityData entity = {};

                entity.name = el["name"].asCString();

                entity.x = el["x"].asInt();
                entity.y = el["y"].asInt();
                entity.width = el["width"].isInt() ? el["width"].asInt() : m_tile_width;
                entity.height = el["height"].isInt() ? el["height"].asInt() : m_tile_height;

                entity.script_path = el["properties"][0]["value"].asCString();
                entity.tag = el["properties"][1]["value"].asInt();

                m_entities.push_back(entity);
            }
        }
    }

    TileMap::~TileMap()
    {
        m_entities.clear();

        delete[] m_foreground.data;
        delete[] m_background.data;
    }

} // namespace tektite
