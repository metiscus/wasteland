#include "wasteland.hpp"
#include "character.hpp"
#include "object.hpp"
#include "utility.hpp"
#include <sstream>
#include <boost/lexical_cast.hpp>

#include <SFML/Window.hpp>

#include <iostream>
#include <cstdio>

Wasteland::Wasteland()
    : should_quit_(false)
    , turn_(0)
    , console_(false)
    , light_radius_(10)
    , step_(false)
{
    window_ = std::make_unique<sf::RenderWindow>(sf::VideoMode(800, 600), "Wasteland");

    CreateSprite(1, "data/grass.png", sf::Vector2f(0.5, 0.5));
    CreateSprite(2, "data/walls.png", sf::Vector2f(0.5, 0.5));
    CreateSprite(3, "data/person.png", sf::Vector2f(0.5, 0.5));
    CreateSprite(4, "data/combat_knife.png", sf::Vector2f(0.5, 0.5));
    CreateSprite(5, "data/45_pistol.png", sf::Vector2f(0.5, 0.5));
    CreateSprite(6, "data/wild_dog.png", sf::Vector2f(0.5, 0.5f));

    player_ = std::make_shared<Character>();
    CharacterTraits traits;
    traits.is_player = 1;
    player_->SetTraits(traits);
    player_->SetPosition(sf::Vector2f(1.0, 1.0));
    player_->SetSpriteId(3);

    //TODO: replace this debug art with something else
    auto red = std::make_shared<sf::Texture>();
    red->create(1,1);
    uint8_t red_b[] = { 255, 100, 100, 255 };
    red->update(red_b);
    red->setRepeated(true);
    textures_[0] = red;

    auto reds = std::make_shared<sf::Sprite>();
    reds->setTexture(*red);
    reds->setScale(sf::Vector2f(32.0f, 32.0f));
    sprites_[0] = reds;

    view_.setCenter(player_->GetPosition());
    window_->setView(view_);

    zoom_ = 1.0f;

    font_.loadFromFile("data/font.ttf");
    
    menu_ = "";

    Object::BuildFromString(std::string("1,3,10,4,Combat Knife,melee,5"));
    Object::BuildFromString(std::string("2,1,10,0,Ration,nutrition,500"));
    Object::BuildFromString(std::string("3,3,18,5,.45 Pistol,melee,1,attack,15,range,80,ammo_cap,8,ammo_type,45"));
    Object::BuildFromString(std::string("4,3,18,0,Teeth,melee,3"));
    
    // inventory window
    equipment_ =  sfg::Window::Create();
    equipment_->SetTitle("Equipment");
    inventory_ = sfg::Box::Create(sfg::Box::Orientation::VERTICAL, 10.f);
    equipment_->Add(inventory_);
    desktop_.Add(equipment_);
}

void Wasteland::CreateSprite(uint32_t id, const std::string& filename, const sf::Vector2f& scale)
{
    auto img = std::make_shared<sf::Image>();
    if(!img->loadFromFile(filename.c_str()))
    {
        std::cerr<<"Failed to load image file: "<<filename<<"\n";
        assert(false);
    }
    else
    {
        images_[id] = img;
        auto tex = std::make_shared<sf::Texture>();
        if(!tex->loadFromImage(*img))
        {
            std::cerr<<"Failed to convert image file: "<<filename<<" to texture\n";
        }
        else
        {
            textures_[id] = tex;

            auto sprite = std::make_shared<sf::Sprite>();
            sprite->setTexture(*tex);
            sprite->scale(scale);
            sprites_[id] = sprite;
        }
    }
}

void Wasteland::Run()
{
    while(!should_quit_)
    {
        ProcessInput();
        
        //TODO: move this to a better place
        while(!actions_.empty())
        {
            const Action& action = actions_.front();
            switch(action.type)
            {
                case Action_PlayerMove:
                    HandlePlayerMovement((PlayerMovement)action.data);
                    break;
                case Action_PlayerPickUp:
                    HandlePickup();
                    break;
                case Action_PlayerDrop:
                    HandlePlayerDropItem(action.data, 1);
                    break;
            }
            actions_.pop();
        }

        UpdateMap();
        Draw();
        step_ = false;
    }

    window_->close();
}

void Wasteland::ProcessInput()
{
    sf::Event event;
    while (window_->pollEvent(event))
    {
        desktop_.HandleEvent(event);
        
        if (event.type == sf::Event::Closed)
        {
            should_quit_ = true;
        }

        if (event.type == sf::Event::KeyPressed)
        {
            // currently i'm unable to cleanly implement commands that arent part of the console
            if(console_)
            {
                //TODO: do this better
                switch(event.key.code)
                {
                    case sf::Keyboard::Tilde:
                        console_ = false;
                        console_command_ = "";
                        break;
                    case sf::Keyboard::Escape:
                        console_ = false;
                        break;
                    case sf::Keyboard::Period:
                        console_command_ += ".";
                        break;
                    case sf::Keyboard::Space:
                        console_command_ += " ";
                        break;
                    case sf::Keyboard::BackSpace:
                        console_command_ = console_command_.substr(0, console_command_.length()-1);
                        break;
                    case sf::Keyboard::Return:
                        DoCommand(console_command_);
                        break;
                    case sf::Keyboard::Dash:
                    case sf::Keyboard::Subtract:
                        console_command_ += "-";
                        break;

                    default:
                        if(event.key.code <= sf::Keyboard::Num9 && event.key.code != sf::Keyboard::Unknown)
                        {
                            std::string letters = "abcdefghijklmnopqrstuvwxyz0123456789";
                            console_command_ += letters[event.key.code];
                        }
                        break;
                }
            }
            else
            {
                Action action;
                switch(event.key.code)
                {
                    case sf::Keyboard::Tilde:
                        console_ = true;
                        console_command_ = "";
                        break;

                    case sf::Keyboard::Numpad4:
                    case sf::Keyboard::A:
                        action.type = Action_PlayerMove;
                        action.data = Player_MoveWest;
                        actions_.push(action);
                        break;
                    case sf::Keyboard::Numpad2:
                    case sf::Keyboard::S:
                        action.type = Action_PlayerMove;
                        action.data = Player_MoveSouth;
                        actions_.push(action);
                        break;
                    case sf::Keyboard::Numpad6:
                    case sf::Keyboard::D:
                        action.type = Action_PlayerMove;
                        action.data = Player_MoveEast;
                        actions_.push(action);
                        break;
                    case sf::Keyboard::Numpad8:
                    case sf::Keyboard::W:
                        action.type = Action_PlayerMove;
                        action.data = Player_MoveNorth;
                        actions_.push(action);
                        break;
                    case sf::Keyboard::Numpad3:
                        action.type = Action_PlayerMove;
                        action.data = Player_MoveSouthEast;
                        actions_.push(action);
                        break;
                    case sf::Keyboard::Numpad1:
                        action.type = Action_PlayerMove;
                        action.data = Player_MoveSouthWest;
                        actions_.push(action);
                        break;
                    case sf::Keyboard::Numpad9:
                        action.type = Action_PlayerMove;
                        action.data = Player_MoveNorthEast;
                        actions_.push(action);
                        break;
                    case sf::Keyboard::Numpad7:
                        action.type = Action_PlayerMove;
                        action.data = Player_MoveNorthWest;
                        actions_.push(action);
                        break;
                    case sf::Keyboard::Equal:
                    case sf::Keyboard::Add:
                        view_.zoom(0.9);
                        break;
                    case sf::Keyboard::Dash:
                    case sf::Keyboard::Subtract:
                        view_.zoom(1.111);
                        break;
                    case sf::Keyboard::G:
                        action.type = Action_PlayerPickUp;
                        action.data = 0;
                        actions_.push(action);
                        break;
                    default:
                        {

                        }
                }

                //TODO: this is a hack
                player_->ChangeFood(-1);
                
                step_ = true;
            }
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

                    //TODO: improve this
                    if(tile.objects.size()>0)
                    {
                        sprite = sprites_[(uint32_t)(*tile.objects.begin()).GetParent()->GetSprite()];
                        sprite->setPosition(sf::Vector2f(x*32, y*32));
                        window_->draw(*sprite);
                    }
                }
            }
        }
    }

    if(map_)
    {
        auto characters = map_->GetCharacters();
        for(auto chr : characters)
        {
            if(map_->GetLit(chr->GetPosition().x, chr->GetPosition().y))
            {
                uint32_t id = chr->GetSpriteId();
                sprites_[id]->setPosition(chr->GetPosition() * 32.0f + sf::Vector2f(16, 16));
                sprites_[id]->setOrigin(sf::Vector2f(32,32));
                auto facing = chr->GetFacing();
                sprites_[id]->setRotation(180.0 / 3.14159 * atan2(facing.x, -facing.y));

                window_->draw(*sprites_[id]);
            }
        }
    }

    window_->draw(*sprites_[3]);

    /// Print text elements
    window_->setView(text_view_);


    desktop_.Update(clock_.restart().asSeconds());
    gui_.Display(*window_);

    // Status line
    sf::Text position(GetStatusLine().c_str(), font_);
    position.setCharacterSize(20);
    window_->draw(position);

    // Console
    if(console_)
    {
        std::string print_console = "Console> ";
        print_console += console_command_;
        sf::Text console(print_console.c_str(), font_);
        console.setCharacterSize(20);
        console.setPosition(sf::Vector2f(0, 100));
        window_->draw(console);

        sf::Text console_output(console_output_.c_str(), font_);
        console_output.setCharacterSize(20);
        console_output.setPosition(sf::Vector2f(50, 150));
        window_->draw(console_output);
    }
    
    sf::Text menu_output(menu_.c_str(), font_);
    menu_output.setCharacterSize(20);
    menu_output.setPosition(sf::Vector2f(50, 200));
    window_->draw(menu_output);

    window_->display();
}

void Wasteland::HandlePlayerInventory()
{
    //TODO: handle dropping specific items
    inventory_->RemoveAll();
    inventory_widgets_.clear();
    std::string inventory_string;
    
    auto inventory_table = sfg::Table::Create();
    inventory_widgets_.push_back(inventory_table);
    inventory_->Pack(inventory_table);

    uint32_t row = 0;
    for(uint32_t type = object_First; type < object_Count; ++type)
    {
        auto objects = player_->GetInventoryObjectsByType((ObjectType)type);
        for(auto obj : objects)
        {
            sf::Rect<sf::Uint32> pos (0, row, 1, 1);

            //Object Image!!!!111
            auto obj_image = sfg::Image::Create();
            obj_image->SetImage(*images_[obj.GetParent()->GetSprite()]);
            inventory_table->Attach(obj_image, pos, sfg::Table::FILL | sfg::Table::EXPAND, sfg::Table::FILL, sf::Vector2f( 10.f, 10.f ) );
            inventory_widgets_.push_back(obj_image);
            ++pos.left;

            //Object Name
            auto obj_label = sfg::Label::Create();
            obj_label->SetText(obj.GetParent()->GetName());
            inventory_table->Attach(obj_label, pos, sfg::Table::FILL | sfg::Table::EXPAND, sfg::Table::FILL, sf::Vector2f( 10.f, 10.f ) );
            inventory_widgets_.push_back(obj_label);
            ++pos.left;
            
            //Object Quantity
            auto obj_qty = sfg::Label::Create();
            std::stringstream ss;
            ss<<obj.GetQuantity();
            obj_qty->SetText(ss.str());
            inventory_table->Attach(obj_qty, pos, sfg::Table::FILL | sfg::Table::EXPAND, sfg::Table::FILL, sf::Vector2f( 10.f, 10.f ) );
            inventory_widgets_.push_back(obj_qty);
            ++pos.left;
            
            //Object Weight
            auto obj_wt = sfg::Label::Create();
            ss.clear();
            ss<<obj.GetParent()->GetWeight() * obj.GetQuantity();
            obj_wt->SetText(ss.str());
            inventory_table->Attach(obj_wt, pos, sfg::Table::FILL | sfg::Table::EXPAND, sfg::Table::FILL, sf::Vector2f( 10.f, 10.f ) );
            inventory_widgets_.push_back(obj_wt);
            ++pos.left;
            
            //Drop button
            auto btn = sfg::Button::Create();
            btn->SetLabel("Drop");
            inventory_table->Attach(btn, pos, sfg::Table::FILL | sfg::Table::EXPAND, sfg::Table::FILL, sf::Vector2f( 10.f, 10.f ) );
            ++pos.left;
            uint32_t id = obj.GetUID();
            uint32_t qty = obj.GetQuantity();
            
            btn->GetSignal( sfg::Button::OnLeftClick ).Connect(
                [this, id] () {
                    Action action;
                    action.type = Action_PlayerDrop;
                    action.data = id;
                    actions_.push(action);
            });
            
            inventory_widgets_.push_back(btn);
            
            ++row;
        }
    }
}

void Wasteland::HandlePlayerDropItem(uint32_t id, uint32_t qty)
{
    auto obj = player_->GetInventoryObject(id);

    auto position = player_->GetPosition();
    auto &tile = map_->Get((uint32_t)position.x, (uint32_t)position.y);
    //TODO: write a function to handle this cleanly
    bool was_there = false;
    for(auto& tgtobj : tile.objects)
    {
        if(tgtobj.GetUID() == id)
        {
            std::cerr<<"Found existing item on drop "<<tgtobj.GetQuantity()<<"\n";
            std::cerr<<"Increasing qty by "<<qty<<"\n";
            tgtobj.ChangeQuantity(qty);
            std::cerr<<"After qty "<<tgtobj.GetQuantity()<<"\n";
            was_there = true;
            break;
        }
    }

    if(!was_there)
    {
        tile.objects.push_back(Object::CreateInstance(id, qty));
    }

    player_->RemoveInventoryObject(id, qty);

    HandlePlayerInventory();
}

void Wasteland::HandlePlayerMovement(PlayerMovement action)
{
    ++turn_;
    auto move_to_pos = player_->GetPosition();
    sf::Vector2f offset;
    switch(action)
    {
        case Player_MoveNorth:
            offset = sf::Vector2f(0.0, -1.0);
            break;
        case Player_MoveEast:
            offset = sf::Vector2f(1.0, 0.0);
            break;
        case Player_MoveSouth:
            offset = sf::Vector2f(0.0, 1.0);
            break;
        case Player_MoveWest:
            offset = sf::Vector2f(-1.0, 0.0);
            break;
        case Player_MoveNorthWest:
            offset = sf::Vector2f(-1.0, -1.0);
            break;
        case Player_MoveNorthEast:
            offset = sf::Vector2f(1.0, -1.0);
            break;
        case Player_MoveSouthWest:
            offset = sf::Vector2f(-1.0, 1.0);
            break;
        case Player_MoveSouthEast:
            offset = sf::Vector2f(1.0, 1.0);
            break;
    }
    move_to_pos += offset;

    auto &tile = map_->Get(move_to_pos.x, move_to_pos.y);
    if(!tile.passable)
    {
        move_to_pos = player_->GetPosition();
    }
    else
    {
        tile.visited = true;
        player_->SetFacing(offset);
    }

    player_->SetPosition(move_to_pos);
    view_.setCenter(move_to_pos * 32.0f);
}

void Wasteland::HandlePickup()
{
    auto position = player_->GetPosition();
    auto &tile = map_->Get((uint32_t)position.x, (uint32_t)position.y);
    if(tile.objects.size() == 0)
    {
        return;
    }

    tile.objects.erase(std::remove_if(tile.objects.begin(), tile.objects.end(), [this](Object::Instance p)
    {
        return player_->AddInventoryObject(p);
    }));
    
    HandlePlayerInventory();
}

void Wasteland::UpdateMap()
{
    if(player_->GetFood() == 0)
    {
        std::cerr<<"Game Over!\n";
        should_quit_ = true;
    }
    
    //TODO: move the logic for character AI out of here once I know what the api will look like
    if(map_ && step_)
    {
        auto characters = map_->GetCharacters();
        for(auto chr : characters)
        {
            if(chr->GetTraits().is_player == 1)
            {
                continue;
            }

            if(chr->GetTraits().beast == 1)
            {
                // do beast ai which in this case is just simple pursuit
                auto player_pos = player_->GetPosition();
                auto my_pos = chr->GetPosition();
                std::vector<bool> litMap = map_->ComputeLighting(my_pos.x, my_pos.y, chr->GetViewRange());
                if(litMap[player_pos.x + player_pos.y*map_->GetWidth()])
                {
                    // we can see the player
                    auto toPlayer = player_pos - my_pos;
                    float distance = sqrtf(toPlayer.x*toPlayer.x + toPlayer.y*toPlayer.y) + 0.001f;
                    toPlayer /= distance;
                    //rework this for ranged
                    if(distance < 2)
                    {
                        // we're close to the player
                        // for now do nothing
                    }
                    else
                    {
                        auto move_to = my_pos + toPlayer;
                        move_to.x = int(roundf(move_to.x));
                        move_to.y = int(roundf(move_to.y));

                        // try to move
                        if(map_->IsPassable(move_to.x, move_to.y))
                        {
                            chr->SetPosition(move_to);
                            chr->SetFacing(sf::Vector2f(int(roundf(toPlayer.x)), int(roundf(toPlayer.y))));
                        }
                    }
                }
            }
        }
    }
}

void Wasteland::UpdateVisited()
{
    auto pos = player_->GetPosition();
    map_->UpdateLighting(pos.x, pos.y, light_radius_);
}

void Wasteland::LoadMap(const std::string& filename)
{
    auto map = Map::Load(filename);
    SetMap(map);
}

void Wasteland::SetMap(std::shared_ptr<Map> map)
{
    map_ = map;
    map_->AddCharacter(player_);
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

//!\brief implements basic console commands for debugging
void Wasteland::DoCommand(const std::string& str)
{
    std::vector<std::string> strings = TokenizeString(str, ' ');
    if(strings[0] == "changefood")
    {
        std::stringstream ss;
        ss<<strings[1];
        int32_t qty = 0;
        ss>>qty;
        player_->ChangeFood(qty);

        console_command_ = "";
    }
    else if(strings[0] == "teleport")
    {
        sf::Vector2f position;
        std::stringstream ss;
        ss<<strings[1];
        ss>>position.x;
        ss.clear();
        ss<<strings[2];
        ss>>position.y;

        player_->SetPosition(position);

        console_command_ = "";
    }
    else if(strings[0] == "setlight")
    {
        std::stringstream ss;
        ss<<strings[1];
        uint32_t radius = light_radius_;
        ss>>radius;
        light_radius_ = radius;

        console_command_ = "";
    }
    else if(strings[0] == "quit")
    {
        should_quit_ = true;
    }
    else if(strings[0] == "savemap")
    {
        map_->Save(strings[1].c_str());
        console_command_ = "";
    }
    else if(strings[0] == "loadmap")
    {
        map_ = Map::Load(strings[1].c_str());
        console_command_ = "";
    }
    else if(strings[0] == "showinv")
    {
        console_output_ = "";
        for(uint32_t type = object_First; type < object_Count; ++type)
        {
            auto objects = player_->GetInventoryObjectsByType((ObjectType)type);
            for(auto obj : objects)
            {
                std::stringstream ss;
                ss<<obj.GetUID()<<" "<<obj.GetQuantity()<<" ";
                ss<<obj.GetParent()->GetName()<<"\n";

                console_output_ += ss.str();
            }
        }
        console_command_ = "";
    }
    else if(strings[0] == "dropinv")
    {
        console_output_ = "";
        uint32_t id = boost::lexical_cast<uint32_t>(strings[1]);
        uint32_t qty = 1;
        if(strings.size() == 3)
        {
            qty = boost::lexical_cast<uint32_t>(strings[2]);
        }

        HandlePlayerDropItem(id, qty);

        console_command_ = "";
    }
    else if(strings[0] == "createobj")
    {
        console_output_ = "";
        uint32_t id = boost::lexical_cast<uint32_t>(strings[1]);
        uint32_t x = boost::lexical_cast<uint32_t>(strings[2]);
        uint32_t y = boost::lexical_cast<uint32_t>(strings[3]);

        auto obj = Object::CreateInstance(id, 1);
        auto &tile = map_->Get(x, y);
        tile.objects.push_back(obj);

        console_command_ = "";
    }
    else if(strings[0] == "consumeobj")
    {
        uint32_t id = boost::lexical_cast<uint32_t>(strings[1]);
        auto obj = player_->GetInventoryObject(id);
        if(obj.GetParent()->GetType() == object_Food && obj.GetQuantity() > 0)
        {
            uint32_t nutrition = obj.GetParent()->GetProperty("nutrition");
            player_->ChangeFood((int32_t)nutrition);
            player_->RemoveInventoryObject(id, 1);
        }
    }
    else
    {
        console_output_ = "";
    }
}

int main(int argc, char** argv)
{
    std::unique_ptr<Wasteland> game(new Wasteland());    
    auto gen = LevelGen();
    gen.Generate(0, 128, 128);
    game->SetMap(gen.GetMap());
    game->Run();
    return 0;
}
