#include "map.hpp"
#include <cassert>
#include <iostream>
#include <fstream>

std::shared_ptr<Map> Map::Load(const std::string& filename)
{
    auto ret = std::make_shared<Map>();
    std::ifstream infile(filename.c_str());
    if(infile.is_open())
    {
        infile>>ret->width;
        infile>>ret->height;
        ret->tiles.resize(ret->width*ret->height);
        ret->lit.resize(ret->width*ret->height);

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
            
            //TODO: make this better
            switch((TileType)type)
            {
                case tile_Ground:
                case tile_Empty:
                    ret->tiles[ii].passable = true;
                    break;

                case tile_Invalid:
                case tile_Wall:
                case tile_Water:
                    ret->tiles[ii].passable = false;
                    break;
                    
                default:
                    assert(false);
            }
            
            ret->lit[ii] = false;
        }
    }
    infile.close();
    
    return ret;
}

std::shared_ptr<Map> Map::Load(std::shared_ptr<sf::Image> img)
{
    auto ret = std::make_shared<Map>();
    sf::Vector2u size = img->getSize();
    ret->tiles.resize(size.x*size.y);
    ret->lit.resize(size.x*size.y);
    ret->width = size.x;
    ret->height= size.y;
    
    for(uint32_t yy=0; yy<size.y; ++yy)
    {
        for(uint32_t xx=0; xx<size.x; ++xx)
        {
            sf::Color pix = img->getPixel(xx, yy);

            ret->tiles[xx+yy*size.x].visited = false;
            
            TileType type = (TileType)(uint32_t)pix.r;

            ret->tiles[xx+yy*size.x].type = type;

            //TODO: make this better
            switch(type)
            {
                case tile_Ground:
                case tile_Empty:
                    ret->tiles[xx+yy*size.x].passable = true;
                    break;

                case tile_Wall:
                case tile_Water:
                    ret->tiles[xx+yy*size.x].passable = false;
                    break;
                    
                default:
                    assert(false);
                    ret->tiles[xx+yy*size.x].type = tile_Invalid;
            }
        }
    }
    return ret;
}

Map::Map()
{
    fov_settings_init(&fov_settings);
    auto apply_lighting = [] (void *map, int x, int y, int dx, int dy, void *src)
    {
        assert(map);
        Map* pMap = (Map*)map;
        if(pMap->OnMap(x, y))
        {
            pMap->SetLit(x, y, true);
        }
    };
    
    auto opaque = [] (void *map, int x, int y)
    {
        assert(map);
        Map* pMap = (Map*)map;
        return pMap->IsOpaque(x,y);
    };
    
    fov_settings_set_opacity_test_function(&fov_settings, opaque);
    fov_settings_set_apply_lighting_function(&fov_settings, apply_lighting);
    fov_settings_set_shape(&fov_settings, FOV_SHAPE_OCTAGON);
}

Map::~Map()
{
    fov_settings_free(&fov_settings);
}

bool Map::IsPassable(uint32_t x, uint32_t y) const
{
    if(OnMap(x,y))
    {
        TileType type = tiles[x+y*width].type;
        switch(type)
        {
            case tile_Ground:
                return true;
                break;
            case tile_Empty:
            case tile_Invalid:
            case tile_Wall:
            case tile_Water:
                return false;
                break;
                
            default:
                assert(false);
                return false;
        }
    }
    return false;
}

bool Map::IsOpaque(uint32_t x, uint32_t y) const
{
    if(OnMap(x,y))
    {
        TileType type = tiles[x+y*width].type;
        switch(type)
        {
            case tile_Water:
            case tile_Empty:
            case tile_Ground:
                return false;
                break;
            
            case tile_Invalid:
            case tile_Wall:
                return true;
                break;
                
            default:
                assert(false);
                return true;
        }
    }
    return true;
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

bool Map::OnMap(uint32_t x, uint32_t y) const
{
    if(x<width && y<height)
        return true;
    return false;
}

void Map::SetLit(uint32_t x, uint32_t y, bool lit)
{
    this->lit[x+y*width] = lit;
    if(lit)
    {
        tiles[x+y*width].visited = true;
    }
}

bool Map::GetLit(uint32_t x, uint32_t y) const
{
    return lit[x+y*width];
}

void Map::UpdateLighting(uint32_t x, uint32_t y, uint32_t radius)
{
    for(uint32_t ii=0; ii<width*height; ++ii)
    {
        lit[ii] = false;
    }

    fov_circle(&fov_settings, this, NULL, x, y, radius);
}
