#ifndef GAME_PAGE_H
#define GAME_PAGE_H

#include <SFML/Graphics.hpp>
#include <functional>

#include "route.hpp"
#include "game.hpp"

class GameRoute : public Route
{
public:
  struct CellsInfo
  {
    unsigned cell_side{0};
    unsigned width_margin{0};
    unsigned height_margin{0};
  };

  using HandleCellCallback = std::function<void(unsigned x, unsigned y)>;

  ~GameRoute() override = default;

  void handle_event(sf::Event &, Router &, sf::RenderWindow &) override;
  void update(float delta_time);
  void render(sf::RenderWindow &);

private:
  unsigned GAME_MARGIN{20};
  unsigned CELL_OUTLINE_BORDER_WIDTH{2};
  sf::Color CELL_BORDER_COLOR{sf::Color::Black};
  sf::Color BACKGROUND_COLOR{sf::Color::White};
  sf::Color DEFAULT_CELL_COLOR{sf::Color{192, 192, 192}}; // grey color
  sf::Color WITH_FLAG_CELL_COLOR{sf::Color::Red};
  sf::Color OPENED_CELL_COLOR{sf::Color::White};
  sf::Color WITH_BOMB_CELL_COLOR{sf::Color::Black};
  sf::Color NUMBER_COLOR{sf::Color::Blue};

  unsigned width{5};
  unsigned height{5};
  unsigned mines_count{5};

  std::unique_ptr<Game> game{init_game()};
  std::unique_ptr<Game> init_game();

  sf::Font &numbers_font{AssetsManager::get_font("assets/fonts/numbers.ttf")};
  sf::Font &emojis_font{AssetsManager::get_font("assets/fonts/emojis.ttf")};

  void draw_background(sf::RenderWindow &) const;
  void draw_game(sf::RenderWindow &) const;
  void draw_cell(unsigned x, unsigned y, sf::RenderWindow &) const;
  void draw_symbol(unsigned x, unsigned y, sf::Text &, sf::RenderWindow &, sf::Vector2f correction_vector = sf::Vector2f{0, 0}) const;
  void handle_cell_click_event(sf::Event, sf::RenderWindow &, HandleCellCallback);

  const CellsInfo get_cells_info(sf::RenderWindow &) const;
};

#endif
