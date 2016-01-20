#include "character.hpp"
#include <unordered_map>

struct CharacterData
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
};

static const uint32_t MaxInventoryWeightPerStrength = 10;

Character::Character()
    : data_(new CharacterData())
{
    data_->strength = 10;
    data_->health = 10;
    data_->max_health = 10;
    data_->radiation = 0;
    data_->name = std::string("(invalid character)");
    data_->inventory_weight = 0;
}

Character::~Character()
{
    
}

const std::string&  Character::GetName() const
{
    return data_->name;
}

const sf::Vector2f& Character::GetPosition() const
{
    return data_->position;
}

const sf::Vector2f& Character::GetFacing() const
{
    return data_->facing;
}

uint32_t Character::GetHealth() const
{
    return data_->health;
}

uint32_t Character::GetMaxHealth() const
{
    return data_->max_health;
}

uint32_t Character::GetRadiation() const
{
    return data_->radiation;
}

void Character::Move(const sf::Vector2f& vec)
{
    SetPosition(GetPosition() + vec);
}

void Character::SetName(const std::string& name)
{
    data_->name = name;
}

void Character::SetPosition(const sf::Vector2f& vec)
{
    data_->position = vec;
}

void Character::SetFacing(const sf::Vector2f& vec)
{
    data_->facing = vec;
}

void Character::SetHealth(uint32_t health)
{
    data_->health = health;
}

void Character::SetMaxHealth(uint32_t max)
{
    data_->max_health = max;
}

void Character::SetRadiation(uint32_t rad)
{
    data_->radiation = rad;
}

bool Character::AddInventoryItem(std::shared_ptr<Object> object)
{
    uint32_t max_carry = data_->strength * MaxInventoryWeightPerStrength;
    if(object->GetWeight() * object->GetQuantity() + data_->inventory_weight > max_carry)
    {
        return false;
    }
    else
    {
        data_->inventory_weight += object->GetWeight() * object->GetQuantity();
    }

    auto itr = data_->inventory.find(object->GetUID());
    if(itr == data_->inventory.end())
    {
        data_->inventory.emplace(std::make_pair(object->GetUID(), object));
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
    ret.reserve(data_->inventory.size());

    for(auto itr : data_->inventory)
    {
        ret.emplace_back(itr.second);
    }
    return ret;
}

void Character::RemoveInventoryItem(uint32_t id, uint32_t qty)
{
    //TODO
}
