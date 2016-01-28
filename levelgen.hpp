#ifndef LEVELGEN_HPP_INCLUDED
#define LEVELGEN_HPP_INCLUDED

#include "map.hpp"
#include <cstdint>
#include <memory>

class LevelGen
{
   MapPtr map_;    

public:
    LevelGen();

    void Generate(uint32_t seed, uint32_t width, uint32_t height);
    MapPtr GetMap();

private:
    void HLine(uint32_t from_x, uint32_t from_y, uint32_t length, TileType type);
    void VLine(uint32_t from_x, uint32_t from_y, uint32_t height, TileType type);

};

#endif
