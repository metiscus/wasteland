#include "character.hpp"
#include <cassert>

static const uint32_t MaxInventoryWeightPerStrength = 10;

Character::Character()
{
    strength_ = 10;
    health_ = 10;
    max_health_ = 10;
    food_ = 1000;
    radiation_ = 0;
    name_ = std::string("(invalid character)");
    inventory_weight_ = 0;
    
    traits_.can_swim = 1;
    traits_.needs_food = 1;
}

Character::~Character()
{
    
}

const std::string&  Character::GetName() const
{
    return name_;
}

const sf::Vector2f& Character::GetPosition() const
{
    return position_;
}

const sf::Vector2f& Character::GetFacing() const
{
    return facing_;
}

uint32_t Character::GetHealth() const
{
    return health_;
}

uint32_t Character::GetMaxHealth() const
{
    return max_health_;
}

uint32_t Character::GetRadiation() const
{
    return radiation_;
}

CharacterTraits Character::GetTraits() const
{
    return traits_;
}

uint32_t Character::GetFood() const
{
    return food_;
}

uint32_t Character::GetSpriteId() const
{
    return sprite_id_;
}

void Character::Move(const sf::Vector2f& vec)
{
    SetPosition(GetPosition() + vec);
}

void Character::SetName(const std::string& name)
{
    name_ = name;
}

void Character::SetPosition(const sf::Vector2f& vec)
{
    position_ = vec;
}

void Character::SetFacing(const sf::Vector2f& vec)
{
    facing_ = vec;
}

void Character::SetHealth(uint32_t health)
{
    health_ = health;
}

void Character::SetMaxHealth(uint32_t max)
{
    max_health_ = max;
}

void Character::SetRadiation(uint32_t rad)
{
    radiation_ = rad;
}

void Character::SetTraits(const CharacterTraits& traits)
{
    traits_ = traits;
}

void Character::SetFood(uint32_t food)
{
    food_ = food;
}

void Character::SetSpriteId(uint32_t id)
{
    sprite_id_ = id;
}

void Character::ChangeFood(int32_t food)
{
    food_ += food;
}

bool Character::AddInventoryObject(std::shared_ptr<Object> object)
{
    uint32_t max_carry = strength_ * MaxInventoryWeightPerStrength;
    if(object->GetWeight() * object->GetQuantity() + inventory_weight_ > max_carry)
    {
        return false;
    }
    else
    {
        inventory_weight_ += object->GetWeight() * object->GetQuantity();
    }

    auto itr = inventory_.find(object->GetUID());
    if(itr == inventory_.end())
    {
        inventory_.emplace(std::make_pair(object->GetUID(), object));
    }
    else
    {
        itr->second->AddQuantity( object->GetQuantity() );
    }
    return true;
}

std::vector<std::shared_ptr<Object> > Character::GetInventoryObjectsByType(ObjectType type)
{
    std::vector<std::shared_ptr<Object> > ret;
    ret.reserve(inventory_.size());

    for(auto itr : inventory_)
    {
        if(itr.second->GetType() == type)
        {
            ret.push_back(itr.second);
        }
    }
    return ret;
}

void Character::RemoveInventoryObject(uint32_t id, uint32_t qty)
{
    auto itr = inventory_.find(id); 
    if(itr != inventory_.end())
    {
        // decrement the quantity or possibly remove the object
        auto obj = itr->second;
        if(obj->GetQuantity() <= qty)
        {
            inventory_.erase(itr);
        }
        else
        {
            obj->SetQuantity(obj->GetQuantity() - qty);
        }
    }
}
