#include "wasteland.hpp"

#include <cstdio>

Wasteland::Wasteland()
    : should_quit_(false)
{
    window_ = std::make_unique<sf::RenderWindow>(sf::VideoMode(800, 600), "Wasteland");
    
    auto red = std::make_shared<sf::Texture>();
    red->create(1,1);
    uint8_t red_b[] = { 255, 0, 0, 255 };
    red->update(red_b);
    red->setRepeated(true);
    textures_[0] = red;

    auto green = std::make_shared<sf::Texture>();
    green->create(1,1);
    uint8_t green_b[] = { 0, 255, 0, 255 };
    green->update(green_b);
    green->setRepeated(true);
    textures_[1] = green;
    
    auto blue = std::make_shared<sf::Texture>();
    blue->create(1,1);
    uint8_t blue_b[] = { 0, 0, 255, 255 };
    blue->update(blue_b);
    blue->setRepeated(true);
    textures_[2] = blue;
    
    auto reds = std::make_shared<sf::Sprite>();
    reds->setTexture(*red);
    reds->setScale(sf::Vector2f(32.0f, 32.0f));
    sprites_[0] = reds;
    
    auto blues = std::make_shared<sf::Sprite>();
    blues->setTexture(*blue);
    blues->setScale(sf::Vector2f(32.0f, 32.0f));
    sprites_[2] = blues;
    
    auto greens = std::make_shared<sf::Sprite>();
    greens->setTexture(*green);
    greens->setScale(sf::Vector2f(32.0f, 32.0f));
    sprites_[1] = greens;
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
    
    if(map_)
    {
        for(uint32_t y = 0; y<map_->GetHeight(); ++y)
        {
            for(uint32_t x = 0; x<map_->GetWidth(); ++x)
            {
                auto &tile = map_->Get(x, y);
                if(tile.visited)
                {
                    sprites_[(uint32_t)tile.type]->setPosition(sf::Vector2f(x*32, y*32));
                    window_->draw(*sprites_[(uint32_t)tile.type]);
                }
            }
        }
    }
    
    window_->display();
}

void Wasteland::LoadMap(const std::string& filename)
{
    map_ = Map::Load(filename);
}

int main(int argc, char** argv)
{
    std::unique_ptr<Wasteland> game(new Wasteland());
    game->LoadMap("data/test.map");
    game->Run();
    return 0;
}
