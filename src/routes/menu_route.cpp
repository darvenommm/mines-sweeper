#include "router.hpp"
#include "menu_route.hpp"
#include "settings_route.hpp"
#include "game_route.hpp"

void MenuRoute::handle_event(sf::Event &event, Router &router)
{
  if (event.type == sf::Event::KeyReleased)
    switch (event.key.scancode)
    {
    case sf::Keyboard::Scancode::F:
      router.change_route(std::make_unique<GameRoute>());
      break;

    case sf::Keyboard::Scancode::J:
      router.change_route(std::make_unique<SettingsRoute>());
      break;

    default:
      break;
    }
}

void MenuRoute::update(float) {}

void MenuRoute::render(sf::RenderWindow &window)
{
  sf::CircleShape shape{50};
  shape.setFillColor(sf::Color::Magenta);

  window.clear();
  window.draw(shape);
  window.display();
}
