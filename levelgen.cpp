#include "levelgen.hpp"

LevelGen::LevelGen()
    : map_(new Map())
{

}

void LevelGen::Generate(uint32_t width, uint32_t height)
{
    map_->Resize(width, height);

}
