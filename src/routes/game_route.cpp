#include "router.hpp"
#include "game_route.hpp"
#include "menu_route.hpp"

void GameRoute::handle_event(sf::Event &event, Router &router)
{
  if (event.type == sf::Event::KeyReleased)
    switch (event.key.scancode)
    {
    case sf::Keyboard::Scancode::Escape:
      router.change_route(std::make_unique<MenuRoute>());
      break;

    default:
      break;
    }
}

void GameRoute::update(float) {}

void GameRoute::render(sf::RenderWindow &window)
{
  sf::RectangleShape square{sf::Vector2f{50, 50}};
  square.setFillColor(sf::Color::Blue);

  window.clear();
  window.draw(square);
  window.display();
}
