#include <cstdint>
#include <memory>
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

class Map
{
private:
    std::unique_ptr<MapData> data_;
    
    Map();

public:
    static std::shared_ptr<Map> Load(const char* filename);
    void Save(const char* filename) const;
    
    const MapTile& Get(uint32_t x, uint32_t y) const;
    MapTile& Get(uint32_t x, uint32_t y);
    
    uint32_t GetWidth() const;
    uint32_t GetHeight() const;
};
