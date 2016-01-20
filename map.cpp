#include "map.hpp"
#include <iostream>
#include <fstream>

Map::Map()
{
}

std::shared_ptr<Map> Map::Load(const std::string& filename)
{
    auto ret = std::make_shared<Map>();
    std::ifstream infile(filename.c_str());
    if(infile.is_open())
    {
        infile>>ret->width;
        infile>>ret->height;
        ret->tiles.resize(ret->width*ret->height);
        uint32_t type, visited;
        for(uint32_t ii=0; ii<ret->width*ret->height; ++ii)
        {
            infile>>type;
            ret->tiles[ii].type = (TileType)type;
            if((TileType)type >= tile_Count)
            {
                std::cerr<<"Saw an invalid tile type "<<type<<"\n";
            }

            infile>>visited;
            ret->tiles[ii].visited = visited;
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
        outfile<<width<<" ";
        outfile<<height<<" ";
        for(uint32_t ii=0; ii<width*height; ++ii)
        {
            outfile<<tiles[ii].type;
            outfile<<tiles[ii].visited;
        }
    }
}
    
const MapTile& Map::Get(uint32_t x, uint32_t y) const
{
    return tiles[x+y*width];
}

MapTile& Map::Get(uint32_t x, uint32_t y)
{
    return tiles[x+y*width];
}

uint32_t Map::GetWidth() const
{
    return width;
}

uint32_t Map::GetHeight() const
{
    return height;
}
