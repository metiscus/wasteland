#include "map.hpp"
#include <fstream>

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

std::shared_ptr<Map> Map::Load(const char* filename)
{
    auto ret = std::make_shared<Map>();
    std::ifstream infile(filename);
    if(infile.is_open())
    {
        infile>>ret->data_->width;
        infile>>ret->data_->height;
        ret->data_->tiles.resize(ret->data_->width*ret->data_->height);
        uint32_t type, visited;
        for(uint32_t ii=0; ii<ret->data_->width*ret->data_->height; ++ii)
        {
            infile>>type;
            ret->data_->tiles[ii].type = (TileType)type;
            infile>>visited;
            ret->data_->tiles[ii].visited = visited;
        }
    }
    infile.close();
    
    return ret;
}

void Map::Save(const char* filename) const
{
    std::ofstream outfile(filename);
    if(outfile.is_open())
    {
        outfile<<data_->width;
        
    }
}
    
const MapTile& Map::Get(uint32_t x, uint32_t y) const
{
    
}

MapTile& Map::Get(uint32_t x, uint32_t y)
{
    
}

uint32_t Map::GetWidth() const
{
    
}

uint32_t Map::GetHeight() const
{
    
}
