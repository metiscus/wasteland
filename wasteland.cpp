#include "wasteland.hpp"
#include "character.hpp"
#include "object.hpp"

#include <SFML/Window.hpp>

#include <iostream>
#include <cstdio>

Wasteland::Wasteland()
    : should_quit_(false)
    , player_(new Character())
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
    
    player_->SetPosition(sf::Vector2f(1.0, 1.0));
    
    auto knife = Object::BuildFromString(std::string("4,1,10,1,Combat Knife"));
    std::cerr<<knife->ToString()<<"\n";
    player_->AddInventoryItem(knife);
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
            
            if (event.type == sf::Event::KeyPressed)
            {
                switch(event.key.code)
                {
                    case sf::Keyboard::Numpad4:
                    case sf::Keyboard::A:
                        HandlePlayerMovement(Player_MoveWest);
                        break;
                    case sf::Keyboard::Numpad2:
                    case sf::Keyboard::S:
                        HandlePlayerMovement(Player_MoveSouth);
                        break;
                    case sf::Keyboard::Numpad6:
                    case sf::Keyboard::D:
                        HandlePlayerMovement(Player_MoveEast);
                        break;
                    case sf::Keyboard::Numpad8:
                    case sf::Keyboard::W:
                        HandlePlayerMovement(Player_MoveNorth);
                        break;
                    default:
                    {

                    }
                }
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
                    auto sprite = sprites_[(uint32_t)tile.type];
                    sprite->setPosition(sf::Vector2f(x*32, y*32));
                    if(!map_->GetLit(x,y))
                    {
                        sprite->setColor(sf::Color(100,100,100));
                    }
                    else
                    {
                        sprite->setColor(sf::Color(255,255,255));
                    }    
                    window_->draw(*sprites_[(uint32_t)tile.type]);
                }
            }
        }
    }
    
    sprites_[0]->setPosition(player_->GetPosition() * 32.0f);
    window_->draw(*sprites_[0]);
    
    window_->display();
}

void Wasteland::HandlePlayerMovement(PlayerMovement action)
{
    auto move_to_pos = player_->GetPosition();
    switch(action)
    {
        case Player_MoveNorth:
            move_to_pos += sf::Vector2f(0.0, -1.0);
            break;
        case Player_MoveEast:
            move_to_pos += sf::Vector2f(1.0, 0.0);
            break;
        case Player_MoveSouth:
            move_to_pos += sf::Vector2f(0.0, 1.0);
            break;
        case Player_MoveWest:
            move_to_pos += sf::Vector2f(-1.0, 0.0);
            break;
    }
    
    auto &tile = map_->Get(move_to_pos.x, move_to_pos.y);
    if(!tile.passable)
    {
        move_to_pos = player_->GetPosition();
    }
    else
    {
        tile.visited = true;
    }
    
    player_->SetPosition(move_to_pos);
    
    UpdateVisited();
}

void Wasteland::UpdateVisited()
{
    auto pos = player_->GetPosition();
    map_->UpdateLighting(pos.x, pos.y, 5);
}

void Wasteland::LoadMap(const std::string& filename)
{
    map_ = Map::Load(filename);
}

void Wasteland::LoadMap(std::shared_ptr<sf::Image> img)
{
    map_ = Map::Load(img);
}

int main(int argc, char** argv)
{
    std::unique_ptr<Wasteland> game(new Wasteland());
    //game->LoadMap("data/test.map");
    auto mapImg = std::make_shared<sf::Image>();
    mapImg->loadFromFile("data/map.png");
    game->LoadMap(mapImg);
    game->Run();
    return 0;
}
