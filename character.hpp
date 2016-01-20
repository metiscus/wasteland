#ifndef CHARACTER_HPP_INCLUDED
#define CHARACTER_HPP_INCLUDED

#include <memory>
#include <string>
#include <unordered_map>

#include <SFML/Graphics.hpp>
#include "object.hpp"

struct CharacterData;

class Character
{
    uint32_t strength;
    uint32_t health;
    uint32_t max_health;
    uint32_t radiation;
    std::string name;
    sf::Vector2f position;
    sf::Vector2f facing;
    uint32_t inventory_weight;
    std::unordered_map<uint32_t, std::shared_ptr<Object> > inventory;

public:
    Character();
    ~Character();
    
    const std::string&  GetName() const;
    const sf::Vector2f& GetPosition() const;
    const sf::Vector2f& GetFacing() const;
    uint32_t GetHealth() const;
    uint32_t GetMaxHealth() const;
    uint32_t GetRadiation() const;

    void Move(const sf::Vector2f& vec);

    void SetName(const std::string& name);
    void SetPosition(const sf::Vector2f& vec);
    void SetFacing(const sf::Vector2f& vec);
    void SetHealth(uint32_t health);
    void SetMaxHealth(uint32_t max);
    void SetRadiation(uint32_t rad);
    
    bool AddInventoryItem(std::shared_ptr<Object> object);
    std::vector<std::shared_ptr<Object> > GetInventoryItemsByType(ObjectType type);
    void RemoveInventoryItem(uint32_t id, uint32_t qty);
};


#endif
