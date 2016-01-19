#include "map.hpp"
#include <iostream>
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

std::shared_ptr<Map> Map::Load(const std::string& filename)
{
    auto ret = std::make_shared<Map>();
    std::ifstream infile(filename.c_str());
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
            if((TileType)type >= tile_Count)
            {
                std::cerr<<"Saw an invalid tile type "<<type<<"\n";
            }

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
        outfile<<data_->width<<" ";
        outfile<<data_->height<<" ";
        for(uint32_t ii=0; ii<data_->width*data_->height; ++ii)
        {
            outfile<<data_->tiles[ii].type;
            outfile<<data_->tiles[ii].visited;
        }
    }
}
    
const MapTile& Map::Get(uint32_t x, uint32_t y) const
{
    return data_->tiles[x+y*data_->width];
}

MapTile& Map::Get(uint32_t x, uint32_t y)
{
    return data_->tiles[x+y*data_->width];
}

uint32_t Map::GetWidth() const
{
    return data_->width;
}

uint32_t Map::GetHeight() const
{
    return data_->height;
}
