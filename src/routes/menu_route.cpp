#include "router.hpp"
#include "menu_route.hpp"
#include "game_route.hpp"

MenuRoute::MenuRoute(Routes current_item) : current_item{static_cast<int>(current_item)} {}

void MenuRoute::handle_event(sf::Event &event, Router &router, sf::RenderWindow &window)
{
  // It's a very large function if this will increase you should separate this monster!
  if (event.type == sf::Event::KeyReleased)
    switch (event.key.scancode)
    {
    case sf::Keyboard::Scancode::Escape:
      window.close();
      break;

    case sf::Keyboard::Scancode::Up:
      if (++current_item > static_cast<int>(Routes::EXIT))
        current_item = static_cast<int>(Routes::GAME);
      break;

    case sf::Keyboard::Scancode::Down:
      if (--current_item < static_cast<int>(Routes::GAME))
        current_item = static_cast<int>(Routes::EXIT);
      break;

    case sf::Keyboard::Scancode::Enter:
      switch (current_item)
      {
      case static_cast<int>(Routes::GAME):
        router.change_route(std::make_unique<GameRoute>());
        break;

      case static_cast<int>(Routes::EXIT):
        window.close();
        break;
      }
      break;

    default:
      break;
    }
}

void MenuRoute::update(float) {}

void MenuRoute::render(sf::RenderWindow &window)
{
  if (items->empty())
    init_items(window);

  const float bg_x{static_cast<float>(window.getSize().x)};
  const float bg_y{static_cast<float>(window.getSize().y)};

  sf::RectangleShape bg{sf::Vector2{bg_x, bg_y}};
  bg.setTexture(&bg_texture);

  window.clear();

  window.draw(bg);

  int item_index = static_cast<int>(Routes::GAME);
  for (auto &item : *items)
  {
    item.setFillColor(item_index == current_item ? sf::Color::Red : sf::Color::Black);
    window.draw(item);
    ++item_index;
  }

  window.display();
}

void MenuRoute::init_items(sf::RenderWindow &window)
{
  sf::Text game_item{};
  game_item.setString(L"Start Game");
  items->push_back(game_item);

  sf::Text settings_item{};
  settings_item.setString(L"Exit");
  items->push_back(settings_item);

  int item_index = static_cast<int>(Routes::GAME);
  float offset = 150.f;
  for (auto &item : *items)
  {
    item.setCharacterSize(50);
    item.setFont(font);

    sf::FloatRect textRect = item.getLocalBounds();
    item.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
    item.setPosition(window.getSize().x / 2.0f, window.getSize().y / 3.0f + item_index * offset);

    ++item_index;
  }
}
