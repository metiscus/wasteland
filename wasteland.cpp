#include "wasteland.hpp"
#include "character.hpp"
#include "object.hpp"

#include <SFML/Window.hpp>

#include <iostream>
#include <cstdio>

Wasteland::Wasteland()
    : should_quit_(false)
    , player_(new Character())
    , turn_(0)
{
    window_ = std::make_unique<sf::RenderWindow>(sf::VideoMode(800, 600), "Wasteland");
   
    //TODO: replace this debug art with something else
    auto red = std::make_shared<sf::Texture>();
    red->create(1,1);
    uint8_t red_b[] = { 255, 100, 100, 255 };
    red->update(red_b);
    red->setRepeated(true);
    textures_[0] = red;

    auto green = std::make_shared<sf::Texture>();
    green->create(1,1);
    uint8_t green_b[] = { 100, 255, 100, 255 };
    green->update(green_b);
    green->setRepeated(true);
    textures_[1] = green;
    
    auto blue = std::make_shared<sf::Texture>();
    blue->create(1,1);
    uint8_t blue_b[] = { 200, 200, 200, 255 };
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
    
    view_.setCenter(player_->GetPosition());
    window_->setView(view_);
    
    zoom_ = 1.0f;
    
    font_.loadFromFile("data/font.ttf");
    
    auto knife = Object::BuildFromString(std::string("4,1,10,1,Combat Knife"));
    std::cerr<<knife->ToString()<<"\n";
    player_->AddInventoryItem(knife);
}

void Wasteland::Run()
{
    while(!should_quit_)
    {
        ProcessInput();
        UpdateMap();
        Draw();
    }
    
    window_->close();
}

void Wasteland::ProcessInput()
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
                case sf::Keyboard::Numpad3:
                    HandlePlayerMovement(Player_MoveSouthEast);
                    break;
                case sf::Keyboard::Numpad1:
                    HandlePlayerMovement(Player_MoveSouthWest);
                    break;
                case sf::Keyboard::Numpad9:
                    HandlePlayerMovement(Player_MoveNorthEast);
                    break;
                case sf::Keyboard::Numpad7:
                    HandlePlayerMovement(Player_MoveNorthWest);
                    break;
                case sf::Keyboard::Add:
                    view_.zoom(0.9);
                    break;
                case sf::Keyboard::Subtract:
                    view_.zoom(1.111);
                    break;
                default:
                    {

                    }
            }

            //TODO: this is a hack
            player_->ChangeFood(-1);
        }
    }
}


void Wasteland::Draw()
{
    if(!window_)
        return;

    window_->clear(sf::Color::Black);
    
    window_->setView(view_);
    
    if(map_)
    {
        UpdateVisited();
        
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
    
    //TODO: draw characters from map as well
    //TODO: draw projectiles from world
    
    sprites_[0]->setPosition(player_->GetPosition() * 32.0f);
    window_->draw(*sprites_[0]);
    
    
    window_->setView(text_view_);
    sf::Text position(GetStatusLine().c_str(), font_);
    position.setCharacterSize(20);
    window_->draw(position);
    
    window_->display();
}

void Wasteland::HandlePlayerMovement(PlayerMovement action)
{
    ++turn_;
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
        case Player_MoveNorthWest:
            move_to_pos += sf::Vector2f(-1.0, -1.0);
            break;
        case Player_MoveNorthEast:
            move_to_pos += sf::Vector2f(1.0, -1.0);
            break;
        case Player_MoveSouthWest:
            move_to_pos += sf::Vector2f(-1.0, 1.0);
            break;
        case Player_MoveSouthEast:
            move_to_pos += sf::Vector2f(1.0, 1.0);
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
    view_.setCenter(move_to_pos * 32.0f);
}

void Wasteland::UpdateMap()
{
    if(player_->GetFood() == 0)
    {
        std::cerr<<"Game Over!\n";
        should_quit_ = true;
    }

    if(map_)
    {
        auto characters = map_->GetCharacters();
        for(auto chr : characters)
        {
            
        }
    }
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
    
    //testing
    map_->SetRadiation(10, 10, 0.5);
    map_->SetRadiation(11, 10, 0.25);
    map_->SetRadiation(12, 10, 0.1);
    map_->SetRadiation(10, 11, 0.5);
    map_->SetRadiation(11, 11, 0.25);
    map_->SetRadiation(12, 11, 0.1);
    map_->SetRadiation(10, 9, 0.5);
    map_->SetRadiation(11, 9, 0.25);
    map_->SetRadiation(12, 9, 0.1);
}

std::string Wasteland::GetStatusLine()
{
    char buffer[1000];
    sprintf(buffer, "Turn:%5d Position:(%3d, %3d) HP:(%3d/%3d)", 
        turn_,
        (uint32_t)player_->GetPosition().x,
        (uint32_t)player_->GetPosition().y,
        (uint32_t)player_->GetHealth(),
        (uint32_t)player_->GetMaxHealth()
    );
    
    std::string ret = buffer;
    
    if(player_->GetFood() < 500)
    {
        ret += " HUNGRY";
    }

    return ret;
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
