#include "levelgen.hpp"
#include <algorithm>
#include <cmath>
#include <random>

LevelGen::LevelGen()
    : map_(new Map())
{

}

MapPtr LevelGen::GetMap()
{
    return map_;
}

void LevelGen::Generate(uint32_t seed, uint32_t width, uint32_t height)
{
    map_->Resize(width, height);
    
    std::default_random_engine generator;
    std::uniform_int_distribution<uint32_t> tile_Type((uint32_t)tile_Ground, (uint32_t)tile_Count-1);
    uint32_t box = (uint32_t)(0.9 * std::min(width, height));
    std::uniform_int_distribution<uint32_t> tile(std::min(5U, box), box);
    for(int yy=0; yy<height; ++yy)
    {
        for(int xx=0; xx<width; ++xx)
        {
            map_->Get(xx, yy).SetFromType(tile_Ground);
        }
    }
    
    HLine(0, 0, width, tile_Wall);
    HLine(0, height-1, width, tile_Wall);
    VLine(0, 0, height, tile_Wall);
    VLine(width-1, 0, height, tile_Wall);

    // Generate a few NPCs
    for(int ii=0; ii<10; ++ii)
    {
        auto dog = std::make_shared<Character>();
        dog->SetPosition(sf::Vector2f(tile(generator) + 1, tile(generator) + 1));
        dog->SetSpriteId(6);
        CharacterTraits traits;
        traits.beast = 1;
        dog->SetTraits(traits);
        map_->AddCharacter(dog);
    }
    
    std::uniform_int_distribution<uint32_t> house(std::min(15U, box), box);
    std::uniform_int_distribution<uint32_t> house_size(7, 15);
    std::uniform_int_distribution<uint32_t> house_item(0, 100);
    // Generate a few houses
    for(int ii=0; ii<5; ++ii)
    {
        uint32_t x = house(generator);
        uint32_t y = house(generator);
        uint32_t hwidth = house_size(generator);
        uint32_t hheight = house_size(generator);
        HLine(x, y, hwidth, tile_Wall);
        HLine(x, y + hheight - 1, hwidth, tile_Wall);
        VLine(x, y, hheight, tile_Wall);
        VLine(x + hwidth-1, y, hheight, tile_Wall);
        
        map_->Get(x, y + hheight /2).SetFromType(tile_Ground);
        
        if(house_item(generator) < 20)
        {
            if(house_item(generator) < 50)
            {
                auto instance = Object::CreateInstance(1, 1);
                map_->Get(x + hwidth/2, y+hheight/2).objects.push_back(instance);
            }
            else
            {
                auto instance = Object::CreateInstance(3, 1);
                map_->Get(x + hwidth/2, y+hheight/2).objects.push_back(instance);
            }
        }
    }
}

void LevelGen::HLine(uint32_t from_x, uint32_t from_y, uint32_t length, TileType type)
{
    for(uint32_t ii=0; ii<length; ++ii)
    {
        if(map_->OnMap(from_x + ii, from_y))
        {
            auto& tile = map_->Get(from_x + ii, from_y);
            tile.SetFromType(type);
        }
    }
}

void LevelGen::VLine(uint32_t from_x, uint32_t from_y, uint32_t height, TileType type)
{
    for(uint32_t ii=0; ii<height; ++ii)
    {
        if(map_->OnMap(from_x, from_y + ii))
        {
            auto& tile = map_->Get(from_x, from_y + ii);
            tile.SetFromType(type);
        }
    }
}
