#include "router.hpp"
#include "game_route.hpp"
#include "menu_route.hpp"

const unsigned GAME_MARGIN = 20U;
const sf::Color CELL_COLOR{192, 192, 192}; // grey color
const unsigned CELL_OUTLINE_WIDTH = 2U;

GameRoute::GameRoute() { init_game(); }

void GameRoute::handle_event(sf::Event &event, Router &router, sf::RenderWindow &)
{
  if (event.type == sf::Event::KeyReleased)
    switch (event.key.scancode)
    {
    case sf::Keyboard::Scancode::Escape:
      router.change_route(std::make_unique<MenuRoute>(MenuRoute{MenuRoute::Routes::GAME}));
      break;

    default:
      break;
    }
}

void GameRoute::update(float) {}

void GameRoute::render(sf::RenderWindow &window)
{
  float window_width{static_cast<float>(window.getSize().x)};
  float window_height{static_cast<float>(window.getSize().y)};
  sf::RectangleShape bg{sf::Vector2f{window_width, window_height}};
  bg.setFillColor(sf::Color::White);

  window.clear();
  window.draw(bg);
  draw_game(window);
  window.display();
}

void GameRoute::init_game() { game = std::make_unique<Game>(10, 10, 20); }

void GameRoute::draw_game(sf::RenderWindow &window) const
{
  unsigned window_width{static_cast<unsigned>(window.getSize().x)};
  unsigned window_height{static_cast<unsigned>(window.getSize().y)};

  unsigned cell_width{(window_width - GAME_MARGIN * 2) / static_cast<unsigned>(width)};
  unsigned cell_height{(window_height - GAME_MARGIN * 2) / static_cast<unsigned>(height)};
  float cell_side{static_cast<float>(std::min(cell_width, cell_height))};

  for (unsigned i{0U}; i < height; ++i)
    for (unsigned j{0U}; j < width; ++j)
    {
      sf::RectangleShape cell{sf::Vector2f{cell_side, cell_side}};
      cell.setPosition(sf::Vector2f{cell_side * j, cell_side * i});
      cell.setOutlineThickness(static_cast<float>(CELL_OUTLINE_WIDTH));
      cell.setOutlineColor(sf::Color::Black);
      cell.setFillColor(CELL_COLOR);

      window.draw(cell);
    }
}
