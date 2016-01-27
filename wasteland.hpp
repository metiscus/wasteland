#ifndef WASTELAND_HPP_INCLUDED
#define WASTELAND_HPP_INCLUDED

#include "map.hpp"
#include <SFML/Graphics.hpp>
#include <string>
#include <unordered_map>
#include "object.hpp"
#include <SFGUI/SFGUI.hpp>
#include <SFGUI/Widgets.hpp>

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
    std::unordered_map<uint32_t, std::shared_ptr<sf::Image> > images_;
    std::unique_ptr<Character> player_;

    sf::View view_;
    sf::View text_view_;
    sf::Font font_;

    float zoom_;
    uint32_t turn_;
    bool console_;
    uint32_t light_radius_;

    std::string console_command_;
    std::string console_output_;
    std::string menu_;
    
    bool step_;
    
    sfg::SFGUI gui_;
    sfg::Desktop desktop_;
    sfg::Window::Ptr equipment_;
    sfg::Box::Ptr inventory_;
    std::vector<sfg::Button::Ptr> buttons_;
    
    sf::Clock clock_;
    
    
public:
    Wasteland();

    void Run();
    void LoadMap(const std::string& filename);
    void LoadMap(std::shared_ptr<sf::Image> img);

private:
    void HandlePlayerMovement(PlayerMovement action);
    void HandlePickup();
    void UpdateVisited();
    std::string GetStatusLine();
    void UpdateMap();
    void ProcessInput();
    void Draw();
    void DoCommand(const std::string& str);

    void CreateSprite(uint32_t id, const std::string& filename, const sf::Vector2f& scale);
    
    void HandlePlayerInventory();
    void HandlePlayerDropItem(uint32_t id, uint32_t qty);
};

#endif
