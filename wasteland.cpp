#include "wasteland.hpp"

#include <cstdio>

Wasteland::Wasteland()
    : should_quit_(false)
{
    window_ = std::make_unique<sf::RenderWindow>(sf::VideoMode(800, 600), "Wasteland");
}

void Wasteland::Run()
{
    while(!should_quit_)
    {
        sf::Event event;
        while (window_->pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                should_quit_ = true;
            }
        }
        
        Draw();
    }
    
    window_->close();
}

void Wasteland::Draw()
{
    if(!window_)
        return;

    window_->clear(sf::Color::Black);
    window_->display();
}

int main(int argc, char** argv)
{
    std::unique_ptr<Wasteland> game(new Wasteland());
    game->Run();
    return 0;
}
