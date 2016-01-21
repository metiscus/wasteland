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

    void Generate(uint32_t width, uint32_t height);
    MapPtr GetMap();

};

#endif
