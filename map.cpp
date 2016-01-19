#include "map.hpp"

struct MapData
{
    MapData()
        : width(0)
        , height(0)
    { }
    uint32_t width;
    uint32_t height;
    std::vector<MapTile> tiles;
};

Map::Map()
    : data_(new MapData())
{
}

static std::shared_ptr<Map> Map::Load(const char* filename);
void Map::Save(const char* filename) const;
    
const MapTile& Map::Get(uint32_t x, uint32_t y) const;
MapTile& Map::Get(uint32_t x, uint32_t y);
    
uint32_t Map::GetWidth() const;
uint32_t Map::GetHeight() const;
