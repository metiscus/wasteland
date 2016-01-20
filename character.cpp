#include "character.hpp"

struct CharacterData
{

};

static const uint32_t MaxInventoryWeightPerStrength = 10;

Character::Character()
{
    strength = 10;
    health = 10;
    max_health = 10;
    radiation = 0;
    name = std::string("(invalid character)");
    inventory_weight = 0;
}

Character::~Character()
{
    
}

const std::string&  Character::GetName() const
{
    return name;
}

const sf::Vector2f& Character::GetPosition() const
{
    return position;
}

const sf::Vector2f& Character::GetFacing() const
{
    return facing;
}

uint32_t Character::GetHealth() const
{
    return health;
}

uint32_t Character::GetMaxHealth() const
{
    return max_health;
}

uint32_t Character::GetRadiation() const
{
    return radiation;
}

void Character::Move(const sf::Vector2f& vec)
{
    SetPosition(GetPosition() + vec);
}

void Character::SetName(const std::string& name)
{
    this->name = name;
}

void Character::SetPosition(const sf::Vector2f& vec)
{
    position = vec;
}

void Character::SetFacing(const sf::Vector2f& vec)
{
    facing = vec;
}

void Character::SetHealth(uint32_t health)
{
    health = health;
}

void Character::SetMaxHealth(uint32_t max)
{
    max_health = max;
}

void Character::SetRadiation(uint32_t rad)
{
    radiation = rad;
}

bool Character::AddInventoryItem(std::shared_ptr<Object> object)
{
    uint32_t max_carry = strength * MaxInventoryWeightPerStrength;
    if(object->GetWeight() * object->GetQuantity() + inventory_weight > max_carry)
    {
        return false;
    }
    else
    {
        inventory_weight += object->GetWeight() * object->GetQuantity();
    }

    auto itr = inventory.find(object->GetUID());
    if(itr == inventory.end())
    {
        inventory.emplace(std::make_pair(object->GetUID(), object));
    }
    else
    {
        itr->second->AddQuantity( object->GetQuantity() );
    }
    return true;
}

std::vector<std::shared_ptr<Object> > Character::GetInventoryItemsByType(ObjectType type)
{
    std::vector<std::shared_ptr<Object> > ret;
    ret.reserve(inventory.size());

    for(auto itr : inventory)
    {
        ret.emplace_back(itr.second);
    }
    return ret;
}

void Character::RemoveInventoryItem(uint32_t id, uint32_t qty)
{
    //TODO
}
