#ifndef WASTELAND_HPP_INCLUDED
#define WASTELAND_HPP_INCLUDED

#include "map.hpp"
#include <SFML/Graphics.hpp>

class Character;

class Wasteland final
{
private:
    std::shared_ptr<Map> map_;
    std::unique_ptr<sf::RenderWindow> window_;

    bool should_quit_;
public:
    Wasteland();
    
    void Run();
    void Draw();
};

#endif
