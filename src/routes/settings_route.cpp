#include "router.hpp"
#include "settings_route.hpp"
#include "menu_route.hpp"

void SettingsRoute::handle_event(sf::Event &event, Router &router)
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

void SettingsRoute::update(float) {}

void SettingsRoute::render(sf::RenderWindow &window)
{
  sf::RectangleShape rectangle{sf::Vector2f{100, 20}};
  rectangle.setFillColor(sf::Color::Red);

  window.clear();
  window.draw(rectangle);
  window.display();
}
