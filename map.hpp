#ifndef MAP_HPP_INCLUDED
#define MAP_HPP_INCLUDED

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

enum TileType
{
    tile_Invalid,
    tile_Ground,
    tile_Wall,
    tile_Water,
    tile_Empty,
    
    tile_Count
};

struct MapTile
{
    TileType type;
    bool visited;
};

struct MapData;

class Map final
{
private:
    std::unique_ptr<MapData> data_;

public:
    Map();
    static std::shared_ptr<Map> Load(const std::string& filename);
    void Save(const char* filename) const;
    
    const MapTile& Get(uint32_t x, uint32_t y) const;
    MapTile& Get(uint32_t x, uint32_t y);
    
    uint32_t GetWidth() const;
    uint32_t GetHeight() const;
};

#endif
