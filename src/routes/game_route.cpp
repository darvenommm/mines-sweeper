#include <string>
#include <iostream>

#include "assets_manager.hpp"
#include "router.hpp"
#include "game_route.hpp"
#include "menu_route.hpp"

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
  window.clear();

  draw_background(window);
  draw_game(window);

  window.display();
}

std::unique_ptr<Game> GameRoute::init_game() { return std::make_unique<Game>(width, height, mines_count); }

void GameRoute::draw_background(sf::RenderWindow &window) const
{
  sf::RectangleShape bg{sf::Vector2f{window.getSize()}};
  bg.setFillColor(BACKGROUND_COLOR);
  window.draw(bg);
}

const GameRoute::CellsInfo GameRoute::get_cells_info(sf::RenderWindow &window) const
{
  unsigned window_width{window.getSize().x};
  unsigned window_height{window.getSize().y};

  unsigned cell_width{(window_width - GAME_MARGIN * 2) / width};
  unsigned cell_height{(window_height - GAME_MARGIN * 2) / height};
  unsigned cell_side{std::min(cell_width, cell_height)};

  unsigned width_rest{window_width - GAME_MARGIN * 2 - cell_side * width};
  unsigned width_margin{(GAME_MARGIN * 2 + width_rest) / 2};

  unsigned height_rest{window_height - GAME_MARGIN * 2 - cell_side * height};
  unsigned height_margin{(GAME_MARGIN * 2 + height_rest) / 2};

  return GameRoute::CellsInfo{
      .cell_side = cell_side,
      .width_margin = width_margin,
      .height_margin = height_margin};
}

void GameRoute::draw_game(sf::RenderWindow &window) const
{
  for (unsigned y{0}; y < height; ++y)
    for (unsigned x{0}; x < width; ++x)
      draw_cell(x, y, window);
}

void GameRoute::draw_cell(unsigned x, unsigned y, sf::RenderWindow &window) const
{
  auto [cell_side, width_margin, height_margin] = get_cells_info(window);
  const Cell &current_cell{game->get_cells()[y * width + x]};

  sf::RectangleShape cell{sf::Vector2f{sf::Vector2u{cell_side, cell_side}}};
  sf::Vector2u position{x * cell_side + width_margin, y * cell_side + height_margin};
  cell.setPosition(sf::Vector2f{position});
  cell.setOutlineThickness(CELL_OUTLINE_BORDER_WIDTH);
  cell.setOutlineColor(CELL_BORDER_COLOR);
  cell.setFillColor(DEFAULT_CELL_COLOR);

  switch (current_cell.get_state())
  {
  case Cell::State::NOT_OPENED:
    window.draw(cell);
    return;

  case Cell::State::WITH_FLAG:
  {
    window.draw(cell);

    sf::Text flag{sf::String{L"🏳️"}, emojis_font};
    flag.setFillColor(sf::Color::Red);
    draw_symbol(x, y, flag, window, sf::Vector2f(-5, -5));
    return;
  }

  case Cell::State::OPENED:
    if (current_cell.get_has_mine())
    {
      cell.setFillColor(WITH_BOMB_CELL_COLOR);
      window.draw(cell);

      sf::Text bomb{sf::String(L"💣"), emojis_font};
      bomb.setFillColor(sf::Color::Red);
      draw_symbol(x, y, bomb, window, sf::Vector2f(-5, -5));
    }
    else
    {
      cell.setFillColor(OPENED_CELL_COLOR);
      window.draw(cell);

      if (current_cell.get_closest_mines_count())
      {
        const sf::String number{std::to_string(current_cell.get_closest_mines_count())};
        sf::Text text_number{number, numbers_font};
        text_number.setFillColor(NUMBER_COLOR);
        draw_symbol(x, y, text_number, window, sf::Vector2f(5, -2));
      }
    }
    return;
  }
}

void GameRoute::draw_symbol(unsigned x, unsigned y, sf::Text &text, sf::RenderWindow &window, sf::Vector2f correction_vector) const
{
  auto [cell_side, width_margin, height_margin] = get_cells_info(window);

  float half_cell{static_cast<float>(cell_side) / 2};
  float x_coordinate{x * cell_side + width_margin + half_cell};
  float y_coordinate{y * cell_side + height_margin + half_cell};

  text.setCharacterSize(half_cell);
  text.setOrigin(sf::Vector2f{half_cell / 2, half_cell / 2});
  text.setPosition(sf::Vector2f{x_coordinate, y_coordinate} + correction_vector);

  window.draw(text);
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
