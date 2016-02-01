#ifndef CHARACTER_HPP_INCLUDED
#define CHARACTER_HPP_INCLUDED

#include <array>
#include <memory>
#include <string>
#include <unordered_map>

#include <SFML/Graphics.hpp>
#include "object.hpp"

struct CharacterData;

struct CharacterTraits
{
    uint64_t is_player : 1;
    uint64_t intelligent : 1;
    uint64_t needs_food : 1;
    uint64_t can_swim : 1;
    uint64_t beast : 1;
    uint64_t padding : 59;
};

class Character;
typedef std::shared_ptr<Character> CharacterPtr;

enum EquipmentSlot
{
    Slot_First,
    Slot_Weapon = Slot_First,
    
    Slot_Count
};

class Character
{
    uint32_t strength_;
    uint32_t health_;
    uint32_t max_health_;
    uint32_t radiation_;
    uint32_t food_;
    uint32_t view_range_;
    std::string name_;
    sf::Vector2f position_;
    sf::Vector2f facing_;
    uint32_t inventory_weight_;
    std::unordered_map<uint32_t, Object::Instance> inventory_;

    CharacterTraits traits_;

    uint32_t sprite_id_;
    
    std::array<ObjectId, Slot_Count> equipement_;


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
    uint32_t GetViewRange() const;

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
    void SetViewRange(uint32_t range);
    void ChangeFood(int32_t food);

    bool AddInventoryObject(Object::Instance object);
    std::vector<Object::Instance> GetInventory();
    std::vector<Object::Instance> GetInventoryObjectsByType(ObjectType type);
    Object::Instance GetInventoryObject(ObjectId uid);
    bool RemoveInventoryObject(ObjectId id, uint32_t qty);
    
    // equipment
    void EquipItem(EquipmentSlot slot, ObjectId id);
    void UnequipItem(EquipmentSlot slot);
    ObjectId GetEquippedItem(EquipmentSlot slot);
};


#endif
