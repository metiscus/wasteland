#ifndef CHARACTER_HPP_INCLUDED
#define CHARACTER_HPP_INCLUDED

#include <memory>
#include <string>
#include <unordered_map>

#include <SFML/Graphics.hpp>
#include "object.hpp"

struct CharacterData;

struct CharacterTraits
{
    uint64_t needs_food : 1;
    uint64_t can_swim : 1;
    uint64_t padding : 63;
};

class Character;
typedef std::shared_ptr<Character> CharacterPtr;

class Character
{
    uint32_t strength_;
    uint32_t health_;
    uint32_t max_health_;
    uint32_t radiation_;
    uint32_t food_;
    std::string name_;
    sf::Vector2f position_;
    sf::Vector2f facing_;
    uint32_t inventory_weight_;
    std::unordered_map<uint32_t, std::shared_ptr<Object> > inventory_;
    
    CharacterTraits traits_;
    
    uint32_t sprite_id_;


public:
    Character();
    ~Character();
    
    const std::string&  GetName() const;
    const sf::Vector2f& GetPosition() const;
    const sf::Vector2f& GetFacing() const;
    uint32_t GetHealth() const;
    uint32_t GetMaxHealth() const;
    uint32_t GetRadiation() const;
    CharacterTraits GetTraits() const;
    uint32_t GetFood() const;
    uint32_t GetSpriteId() const;

    void Move(const sf::Vector2f& vec);

    void SetSpriteId(const uint32_t id);
    void SetName(const std::string& name);
    void SetPosition(const sf::Vector2f& vec);
    void SetFacing(const sf::Vector2f& vec);
    void SetHealth(uint32_t health);
    void SetMaxHealth(uint32_t max);
    void SetRadiation(uint32_t rad);
    void SetTraits(const CharacterTraits& traits);
    void SetFood(uint32_t food);
    void ChangeFood(int32_t food);
    
    bool AddInventoryItem(std::shared_ptr<Object> object);
    std::vector<std::shared_ptr<Object> > GetInventoryItemsByType(ObjectType type);
    void RemoveInventoryItem(uint32_t id, uint32_t qty);
};


#endif
