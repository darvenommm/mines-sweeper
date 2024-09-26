#include "router.hpp"
#include "game_route.hpp"
#include "menu_route.hpp"

const unsigned GAME_MARGIN = 20;
const sf::Color CELL_COLOR{192, 192, 192}; // grey color
const unsigned CELL_OUTLINE_WIDTH = 2;

void GameRoute::handle_event(sf::Event &event, Router &router, sf::RenderWindow &window)
{
  if (event.type == sf::Event::KeyReleased)
    switch (event.key.scancode)
    {
    case sf::Keyboard::Scancode::Escape:
      router.change_route(std::make_unique<MenuRoute>(MenuRoute{MenuRoute::Routes::GAME}));
      return;

    default:
      return;
    }

  if (event.type == sf::Event::MouseButtonReleased)
    handle_opening_mine_event(event, window);
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

std::unique_ptr<Game> GameRoute::init_game() { return std::make_unique<Game>(width, height, mines_count); }

const GameRoute::DrawingInfo GameRoute::get_drawing_info(sf::RenderWindow &window) const
{
  unsigned window_width{window.getSize().x};
  unsigned window_height{window.getSize().y};

  unsigned cell_width{(window_width - GAME_MARGIN * 2) / static_cast<unsigned>(width)};
  unsigned cell_height{(window_height - GAME_MARGIN * 2) / static_cast<unsigned>(height)};
  float cell_side{static_cast<float>(std::min(cell_width, cell_height))};

  float width_rest{window_width - GAME_MARGIN * 2 - cell_side * width};
  float width_margin{static_cast<float>(GAME_MARGIN * 2 + width_rest) / 2};

  float height_rest{window_height - GAME_MARGIN * 2 - cell_side * height};
  float height_margin{static_cast<float>(GAME_MARGIN * 2 + height_rest) / 2};

  return GameRoute::DrawingInfo{
      .cell_side = cell_side,
      .width_margin = width_margin,
      .height_margin = height_margin};
}

void GameRoute::draw_game(sf::RenderWindow &window) const
{
  auto info{get_drawing_info(window)};

  for (unsigned i{0}; i < height; ++i)
    for (unsigned j{0}; j < width; ++j)
    {
      sf::RectangleShape cell{sf::Vector2f{info.cell_side, info.cell_side}};
      cell.setPosition(sf::Vector2f{info.cell_side * j + info.width_margin, info.cell_side * i + info.height_margin});
      cell.setOutlineThickness(static_cast<float>(CELL_OUTLINE_WIDTH));
      cell.setOutlineColor(sf::Color::Black);

      auto mine_status{game->get_mines()[i * width + j].get_status()};
      switch (mine_status)
      {
      case Mine::Status::NOT_OPENED:
        cell.setFillColor(CELL_COLOR);
        break;

      case Mine::Status::WITH_FLAG:
        cell.setFillColor(sf::Color::Red);
        break;

      case Mine::Status::WITH_BOMB:
        cell.setFillColor(sf::Color::Black);
        break;

      case Mine::Status::WITHOUT_BOMB:
        cell.setFillColor(sf::Color::Magenta);
        break;
      }

      window.draw(cell);
    }
}

void GameRoute::handle_opening_mine_event(sf::Event &event, sf::RenderWindow &window)
{
  auto info{get_drawing_info(window)};

  float mouse_click_x{static_cast<float>(event.mouseButton.x)};
  float mouse_click_y{static_cast<float>(event.mouseButton.y)};

  float width_cell{(mouse_click_x - info.width_margin) / info.cell_side};
  float height_cell{(mouse_click_y - info.height_margin) / info.cell_side};

  bool correct_width_cell = width_cell > 0 && width_cell < width;
  bool correct_height_cell = height_cell > 0 && height_cell < height;

  if (correct_width_cell && correct_height_cell)
    game->open_mine(static_cast<unsigned>(width_cell), static_cast<unsigned>(height_cell));
}
