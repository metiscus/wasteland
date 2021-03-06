#include "levelgen.hpp"
#include <algorithm>
#include <cmath>
#include <random>
#include "map.hpp"
#include <SFML/Graphics.hpp>

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
    
    std::random_device rd;
    
    std::default_random_engine generator(seed);
    if(seed == 0)
    {
        generator = std::default_random_engine(rd());
    }
    std::uniform_int_distribution<uint32_t> tile_Type((uint32_t)tile_Ground, (uint32_t)tile_Count-1);
    uint32_t box = (uint32_t)(0.9 * std::min(width, height));
    std::uniform_int_distribution<uint32_t> tile(std::min(5U, box), box);
    for(uint32_t yy=0; yy<height; ++yy)
    {
        for(uint32_t xx=0; xx<width; ++xx)
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
        dog->SetMaxHealth(60);
        dog->SetHealth(60);
        dog->SetTraits(traits);
        
        if(tile(generator) % 2 == 0)
        {
			//Dogs can "carry" some meat to drop
			Object::Instance meat = Object::CreateInstance(6, 1);
			dog->AddInventoryObject(meat);
		}
        map_->AddCharacter(dog);
    }
    
    std::uniform_int_distribution<uint32_t> rng_house(std::min(15U, box), box);
    std::uniform_int_distribution<uint32_t> house_size(7, 15);
    std::uniform_int_distribution<uint32_t> house_item(0, 100);
    
    std::list<sf::Rect<uint32_t> > houses;
    // Generate a few houses
    for(int ii=0; ii<5; ++ii)
    {
        bool valid;

        sf::Rect<uint32_t> house;
        do
        {
            valid = true;
            house = sf::Rect<uint32_t>(rng_house(generator), rng_house(generator), house_size(generator), house_size(generator));
            for(auto itr=houses.begin(); itr !=houses.end(); ++itr)
            {
                if(itr->intersects(house))
                {
                    valid = false;
                    break;
                }
            }

        } while(!valid);
        
        houses.push_front(house);

        HLine(house.left, house.top, house.width, tile_Wall);
        HLine(house.left, house.top + house.height - 1, house.width, tile_Wall);
        VLine(house.left, house.top, house.height, tile_Wall);
        VLine(house.left + house.width-1, house.top, house.height, tile_Wall);
        
        map_->Get(house.left, house.top + house.height /2).SetFromType(tile_Ground);
        
        for(uint32_t yy=1; yy<house.height-1; ++yy)
        {
            for(uint32_t xx=1; xx<house.width-1; ++xx)
            {
                auto& tile = map_->Get(house.left + xx, house.top+yy);
                tile.group = ii+1;
                tile.sprite = Map::GetTileMapping("floor");
            }
        }
        
        if(house_item(generator) < 40)
        {
            if(house_item(generator) < 50)
            {
                auto instance = Object::CreateInstance(1, 1);
                map_->Get(house.left + house.width/2, house.top+house.height/2).objects.push_back(instance);
            }
            else
            {
                auto instance = Object::CreateInstance(3, 1);
                map_->Get(house.left + house.width/2, house.top+house.height/2).objects.push_back(instance);
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
