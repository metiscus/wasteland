#ifndef CHARACTER_HPP_INCLUDED
#define CHARACTER_HPP_INCLUDED

#include <memory>
#include <string>

#include <SFML/Graphics.hpp>

struct CharacterData;

class Character
{
    std::unique_ptr<CharacterData> data_;
    
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
};


#endif
