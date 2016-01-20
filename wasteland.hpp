#ifndef WASTELAND_HPP_INCLUDED
#define WASTELAND_HPP_INCLUDED

#include "map.hpp"
#include <SFML/Graphics.hpp>
#include <string>
#include <unordered_map>

class Character;

enum PlayerMovement
{
    Player_MoveNorth,
    Player_MoveSouth,
    Player_MoveEast,
    Player_MoveWest,
    Player_MoveNorthWest,
    Player_MoveNorthEast,
    Player_MoveSouthWest,
    Player_MoveSouthEast
};

class Wasteland final
{
private:
    std::shared_ptr<Map> map_;
    std::unique_ptr<sf::RenderWindow> window_;

    bool should_quit_;
    
    std::unordered_map<uint32_t, std::shared_ptr<sf::Sprite> > sprites_;
    
    std::unordered_map<uint32_t, std::shared_ptr<sf::Texture> > textures_;

    std::unique_ptr<Character> player_;
    
    sf::View view_;
    
    float zoom_;

public:
    Wasteland();
    
    void Run();
    void Draw();
    void LoadMap(const std::string& filename);
    void LoadMap(std::shared_ptr<sf::Image> img);
    
private:
    void HandlePlayerMovement(PlayerMovement action);
    void UpdateVisited();
};

#endif
