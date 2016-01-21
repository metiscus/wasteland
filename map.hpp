#ifndef MAP_HPP_INCLUDED
#define MAP_HPP_INCLUDED

#include "character.hpp"
#include <cstdint>
#include <memory>
#include <string>
#include <SFML/Graphics.hpp>
#include <vector>
#include <list>
#include "fov.h"
#include "object.hpp"

enum TileType
{
    tile_Invalid,
    tile_Ground,
    tile_Wall,
    tile_Water,
    tile_Empty,
    
    tile_Count
};

struct MapTile
{
    MapTile();
    TileType type;
    bool visited;
    bool passable;
    std::list<ObjectPtr> objects;
    float radiation;
};

class Map final
{
private:
    uint32_t width;
    uint32_t height;
    std::vector<MapTile> tiles;
    std::vector<bool> lit;
    std::list<CharacterPtr> characters;

    fov_settings_type fov_settings;
public:
    Map();
    ~Map();
    static std::shared_ptr<Map> Load(std::shared_ptr<sf::Image> img);
    static std::shared_ptr<Map> Load(const std::string& filename);
    void Save(const char* filename) const;
    
    const MapTile& Get(uint32_t x, uint32_t y) const;
    MapTile& Get(uint32_t x, uint32_t y);
    
    bool GetLit(uint32_t x, uint32_t y) const;
    float GetRadiation(uint32_t x, uint32_t y) const;
    
    bool OnMap(uint32_t x, uint32_t y) const;
    
    uint32_t GetWidth() const;
    uint32_t GetHeight() const;
    
    void UpdateLighting(uint32_t x, uint32_t y, uint32_t radius);
    void SetLit(uint32_t x, uint32_t y, bool lit);
    void SetRadiation(uint32_t x, uint32_t y, float radiation);
    
    bool IsPassable(uint32_t x, uint32_t y) const;
    bool IsOpaque(uint32_t x, uint32_t y) const;

    std::list<CharacterPtr>& GetCharacters();
    
    void AddCharacter(CharacterPtr ptr);
    void RemoveCharacter(CharacterPtr ptr);

    void Resize(uint32_t x, uint32_t y);
};

typedef std::shared_ptr<Map> MapPtr;

#endif
