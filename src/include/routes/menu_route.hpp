#ifndef MENU_PAGE_H
#define MENU_PAGE_H

#include <SFML/Graphics.hpp>

#include "assets_manager.hpp"
#include "route.hpp"

class MenuRoute : public Route
{
public:
  enum class Routes : int
  {
    GAME,
    EXIT,
  };

  MenuRoute(Routes current_item = Routes::GAME);
  ~MenuRoute() override = default;
  MenuRoute(MenuRoute &&other) noexcept = default;
  MenuRoute &operator=(MenuRoute &&other) noexcept = default;

  void handle_event(sf::Event &, Router &, sf::RenderWindow &) override;
  void update(float delta_time);
  void render(sf::RenderWindow &);

private:
  sf::Texture &bg_texture{AssetsManager::get_texture("assets/images/menu-bg.jpeg")};
  sf::Font &font{AssetsManager::get_font("assets/fonts/start_menu.ttf")};
  std::unique_ptr<std::vector<sf::Text>> items{std::make_unique<std::vector<sf::Text>>()};
  int current_item{static_cast<int>(Routes::GAME)};

  void init_items(sf::RenderWindow &);
};

#endif
