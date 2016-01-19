#ifndef CHARACTER_HPP_INCLUDED
#define CHARACTER_HPP_INCLUDED

#include <memory>
#include <string>

struct CharacterData;

class Character
{
    std::unique_ptr<CharacterData> data_;
    
public:
    Character();
    
    const std::string&  GetName() const;
    const sf::Vector2f& GetPosition() const;
    const sf::Vector2f& GetFacing() const;
    uint32_t GetHealth() const;
    uint32_t GetMaxHealth() const;
    uint32_t GetRadiation() const;
};


#endif
