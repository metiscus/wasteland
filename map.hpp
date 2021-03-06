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
#include <map>

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
    std::list<Object::Instance> objects;
    float radiation;
    uint32_t group;
    uint32_t sprite;
    
    void SetFromType(TileType type);
    void AddObject(ObjectId id, uint32_t qty);
    void RemoveObject(ObjectId id, uint32_t qty);
};

class Map final
{
private:
    uint32_t width_;
    uint32_t height_;
    std::vector<MapTile> tiles_;
    std::vector<bool> lit_;
    std::list<CharacterPtr> characters_;

    fov_settings_type fov_settings_;
    
    static std::unordered_map<std::string, uint32_t> sprite_map_;
public:
    Map();
    ~Map();
    static std::shared_ptr<Map> Load(std::shared_ptr<sf::Image> img);
    static std::shared_ptr<Map> Load(const std::string& filename);
    void Save(const char* filename) const;
    
    static void AddTileMapping(const std::string& name, uint32_t id);
    static uint32_t GetTileMapping(const std::string& name);
    
    const MapTile& Get(uint32_t x, uint32_t y) const;
    MapTile& Get(uint32_t x, uint32_t y);
    
    bool GetLit(uint32_t x, uint32_t y) const;
    float GetRadiation(uint32_t x, uint32_t y) const;
    
    bool OnMap(uint32_t x, uint32_t y) const;
    
    uint32_t GetWidth() const;
    uint32_t GetHeight() const;
    
    void UpdateLighting(uint32_t x, uint32_t y, uint32_t radius);
    std::vector<bool> ComputeLighting(uint32_t x, uint32_t y, uint32_t radius);
    void SetLit(uint32_t x, uint32_t y, bool lit);
    void SetRadiation(uint32_t x, uint32_t y, float radiation);
    
    bool IsPassable(uint32_t x, uint32_t y) const;
    bool IsOccupied(uint32_t x, uint32_t y) const;
    bool IsOpaque(uint32_t x, uint32_t y) const;

    std::list<CharacterPtr>& GetCharacters();
    
    void AddCharacter(CharacterPtr ptr);
    void RemoveCharacter(CharacterPtr ptr);
    CharacterPtr GetCharacter(uint32_t x, uint32_t y);

    void Resize(uint32_t x, uint32_t y);
};

typedef std::shared_ptr<Map> MapPtr;

#endif
