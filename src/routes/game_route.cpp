#include <format>
#include <iostream>

#include "assets_manager.hpp"
#include "router.hpp"
#include "game_route.hpp"
#include "menu_route.hpp"

const unsigned GAME_MARGIN = 20;
const sf::Color CELL_COLOR{192, 192, 192}; // grey color
const unsigned CELL_OUTLINE_WIDTH = 2;

void GameRoute::handle_event(sf::Event &event, Router &router, sf::RenderWindow &window)
{
  switch (event.type)
  {
  case sf::Event::KeyReleased:
    if (event.key.scancode == sf::Keyboard::Scancode::Escape)
      router.change_route(std::make_unique<MenuRoute>(MenuRoute{MenuRoute::Routes::GAME}));
    break;

  case sf::Event::MouseButtonReleased:
  {
    sf::Mouse::Button button{event.mouseButton.button};

    if (button == sf::Mouse::Left)
    {
      GameRoute::HandleCellCallback leftClickCallback{
          [this](unsigned x, unsigned y)
          {
            game->open_cell(x, y);
          }};
      handle_cell_click_event(event, window, leftClickCallback);
    }
    else if (button == sf::Mouse::Right)
    {
      GameRoute::HandleCellCallback rightClickCallback{
          [this](unsigned x, unsigned y)
          {
            game->toggle_cell_flag(x, y);
          }};
      handle_cell_click_event(event, window, rightClickCallback);
    }
    break;
  }

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

std::unique_ptr<Game> GameRoute::init_game() { return std::make_unique<Game>(width, height, mines_count); }

const GameRoute::CellsInfo GameRoute::get_cells_info(sf::RenderWindow &window) const
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

  return GameRoute::CellsInfo{
      .cell_side = cell_side,
      .width_margin = width_margin,
      .height_margin = height_margin};
}

void GameRoute::draw_game(sf::RenderWindow &window) const
{
  auto info{get_cells_info(window)};
  auto numbers{std::make_unique<std::vector<sf::Text>>()};

  for (unsigned i{0}; i < height; ++i)
    for (unsigned j{0}; j < width; ++j)
    {
      sf::RectangleShape cell_shape{sf::Vector2f{info.cell_side, info.cell_side}};
      cell_shape.setPosition(sf::Vector2f{info.cell_side * j + info.width_margin, info.cell_side * i + info.height_margin});
      cell_shape.setOutlineThickness(static_cast<float>(CELL_OUTLINE_WIDTH));
      cell_shape.setOutlineColor(sf::Color::Black);

      Cell cell{game->get_cells()[i * width + j]};
      switch (cell.get_state())
      {
      case Cell::State::NOT_OPENED:
        cell_shape.setFillColor(CELL_COLOR);
        break;

      case Cell::State::WITH_FLAG:
        cell_shape.setFillColor(sf::Color::Red);
        break;

      case Cell::State::OPENED:
        if (cell.get_has_mine())
          cell_shape.setFillColor(sf::Color::Black);
        else
        {
          sf::String string{std::format(L"{}", cell.get_closest_mines_count())};
          sf::Font &font{AssetsManager::get_font("assets/fonts/font.ttf")};

          sf::Text number{string, font, 50};
          number.setPosition(cell_shape.getPosition() + sf::Vector2f{25, 25});
          number.setFillColor(sf::Color::Magenta);

          numbers->push_back(number);
        }
        break;
      }

      window.draw(cell_shape);

      for (const auto &number : *numbers)
        window.draw(number);
    }
}

void GameRoute::handle_cell_click_event(sf::Event event, sf::RenderWindow &window, GameRoute::HandleCellCallback callback)
{
  auto info{get_cells_info(window)};

  float mouse_click_x{static_cast<float>(event.mouseButton.x)};
  float mouse_click_y{static_cast<float>(event.mouseButton.y)};

  float width_cell{(mouse_click_x - info.width_margin) / info.cell_side};
  float height_cell{(mouse_click_y - info.height_margin) / info.cell_side};

  bool correct_width_cell = width_cell > 0 && width_cell < width;
  bool correct_height_cell = height_cell > 0 && height_cell < height;

  if (correct_width_cell && correct_height_cell)
    callback(static_cast<unsigned>(width_cell), static_cast<unsigned>(height_cell));
}
