#include "map.hpp"
#include <algorithm>
#include <cassert>
#include <iostream>
#include <fstream>
#include <rapidxml.hpp>
#include <rapidxml_print.hpp>
#include <rapidxml_utils.hpp>
#include <boost/lexical_cast.hpp>

MapTile::MapTile()
{
    type      = tile_Invalid;
    visited   = false;
    passable  = false;
    radiation = 0.0f;
}

inline void DeserializeTile(const rapidxml::xml_node<> *tile_node, MapTile& tile);
inline void SerializeTile(rapidxml::xml_node<> *map_node, const MapTile& tile);

std::shared_ptr<Map> Map::Load(const std::string& filename)
{
    using namespace rapidxml;
    using namespace boost;

    auto ret = std::make_shared<Map>();

    rapidxml::file<> infile(filename.c_str());
    xml_document<> doc;
    doc.parse<0>(infile.data());

    xml_node<> *map = doc.first_node("map");
    if(map)
    {
        ret->width = lexical_cast<uint32_t>(map->first_attribute("width")->value());
        ret->height = lexical_cast<uint32_t>(map->first_attribute("height")->value());

        ret->lit.resize(ret->width*ret->height);
        ret->tiles.resize(ret->width*ret->height);
        auto tileNode = map->first_node();
        for(uint32_t ii=0; ii<ret->width*ret->height; ++ii)
        {
            DeserializeTile(tileNode, ret->tiles[ii]);
            tileNode = tileNode->next_sibling();
        }
    }

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
    //fov_settings_set_shape(&fov_settings, FOV_SHAPE_OCTAGON);
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

float Map::GetRadiation(uint32_t x, uint32_t y) const
{
    return tiles[x+y*width].radiation;
}

void Map::SetRadiation(uint32_t x, uint32_t y, float radiation)
{
    tiles[x+y*width].radiation = radiation;
}

void Map::Save(const char* filename) const
{
    using namespace rapidxml;
    xml_document<> doc;
    xml_node<> *root = doc.allocate_node(node_element, doc.allocate_string("map"));
    doc.append_node(root);

    xml_attribute<> *attr = doc.allocate_attribute(doc.allocate_string("width"), boost::lexical_cast<std::string>(width).c_str());
    root->append_attribute(attr);

    attr = doc.allocate_attribute(doc.allocate_string("height"), boost::lexical_cast<std::string>(height).c_str());
    root->append_attribute(attr);

    std::ofstream outfile(filename);

    if(outfile.is_open())
    {
        for(uint32_t ii=0; ii<width*height; ++ii)
        {
            SerializeTile(root, tiles[ii]);
        }
    }

    std::string xml_as_string;
    print(std::back_inserter(xml_as_string), doc);
    outfile<<xml_as_string;
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

    lit[x + y*width] = true;
}

std::list<CharacterPtr>& Map::GetCharacters()
{
    return characters;
}

void Map::AddCharacter(CharacterPtr ptr)
{
    characters.push_back(ptr);
}

void Map::RemoveCharacter(CharacterPtr ptr)
{
    characters.erase(std::remove(characters.begin(), characters.end(), ptr), characters.end());
}

void Map::Resize(uint32_t x, uint32_t y)
{
    tiles.resize(x*y);
    lit.resize(x*y);
    width = x;
    height = y;
}

//TODO: there absolutely has to be a better way to handle this
inline void DeserializeTile(const rapidxml::xml_node<> *tile_node, MapTile& tile)
{
    using namespace rapidxml;
    using namespace boost;
    assert(tile_node);
    tile.type = (TileType)lexical_cast<uint32_t>(tile_node->first_attribute("type")->value());
    tile.visited = lexical_cast<bool>(tile_node->first_attribute("visited")->value());
    tile.passable = lexical_cast<bool>(tile_node->first_attribute("passable")->value());
    tile.radiation = lexical_cast<float>(tile_node->first_attribute("radiation")->value());

    xml_node<> *objectNode = tile_node->first_node("object_instance");
    for( ; objectNode != nullptr; objectNode = objectNode->next_sibling("object_instance"))
    {
        uint32_t uid;
        uint32_t qty;
        uid = lexical_cast<uint32_t>(objectNode->first_attribute("uid")->value());
        qty = lexical_cast<uint32_t>(objectNode->first_attribute("quantity")->value());
        tile.objects.push_back(Object::CreateInstance(uid, qty));
    }
}

inline void SerializeTile(rapidxml::xml_node<> *map_node, const MapTile& tile)
{
    using namespace rapidxml;
    using namespace boost;

    assert(map_node);
    auto doc = map_node->document();
    auto node = doc->allocate_node(node_element, doc->allocate_string("tile"));
    map_node->append_node(node);

    auto type_str = doc->allocate_string(lexical_cast<std::string>((uint32_t)tile.type).c_str());
    auto attr = doc->allocate_attribute(doc->allocate_string("type"), type_str);
    node->append_attribute(attr);

    auto visited_str = doc->allocate_string(lexical_cast<std::string>(tile.visited).c_str());
    attr = doc->allocate_attribute(doc->allocate_string("visited"), visited_str);
    node->append_attribute(attr);

    auto passable_str = doc->allocate_string(lexical_cast<std::string>(tile.passable).c_str());
    attr = doc->allocate_attribute(doc->allocate_string("passable"), passable_str);
    node->append_attribute(attr);

    auto radiation_str = doc->allocate_string(lexical_cast<std::string>(tile.radiation).c_str());
    attr = doc->allocate_attribute(doc->allocate_string("radiation"), radiation_str);
    node->append_attribute(attr);

    // Write out all object instances
    for(auto itr : tile.objects)
    {
        auto instance = doc->allocate_node(node_element, doc->allocate_string("object_instance"));
        node->append_node(instance);
        auto uid_str = doc->allocate_string(lexical_cast<std::string>((uint32_t)itr.GetUID()).c_str());
        auto uid_attr = doc->allocate_attribute(doc->allocate_string("uid"), uid_str);
        instance->append_attribute(uid_attr);

        auto qty_str = doc->allocate_string(lexical_cast<std::string>((uint32_t)itr.GetQuantity()).c_str());
        auto qty_attr = doc->allocate_attribute(doc->allocate_string("quantity"), qty_str);
        instance->append_attribute(qty_attr);
    }
}
