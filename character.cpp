#include "character.hpp"

struct CharacterData
{
    uint32_t health;
    uint32_t max_health;
    uint32_t radiation;
    std::string name;
    sf::Vector2f position;
    sf::Vector2f facing;
};

Character::Character()
    : data_(new CharacterData())
{
    
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
