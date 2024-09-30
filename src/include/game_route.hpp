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
    float cell_side{0};
    float width_margin{0};
    float height_margin{0};
  };

  using HandleCellCallback = std::function<void(unsigned x, unsigned y)>;

  ~GameRoute() override = default;

  void handle_event(sf::Event &, Router &, sf::RenderWindow &) override;
  void update(float delta_time);
  void render(sf::RenderWindow &);

private:
  unsigned width{10};
  unsigned height{10};
  unsigned mines_count{25};

  std::unique_ptr<Game> game{init_game()};
  std::unique_ptr<Game> init_game();

  void draw_game(sf::RenderWindow &) const;
  void handle_cell_click_event(sf::Event, sf::RenderWindow &, HandleCellCallback);

  const CellsInfo get_cells_info(sf::RenderWindow &) const;
};

#endif
