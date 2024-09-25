#ifndef GAME_PAGE_H
#define GAME_PAGE_H

#include <SFML/Graphics.hpp>

#include "route.hpp"
#include "game.hpp"

class GameRoute : public Route
{
public:
  struct DrawingInfo
  {
    float cell_side{0};
    float width_margin{0};
    float height_margin{0};
  };

  GameRoute();
  ~GameRoute() override = default;

  void handle_event(sf::Event &, Router &, sf::RenderWindow &) override;
  void update(float delta_time);
  void render(sf::RenderWindow &);

private:
  unsigned width{10};
  unsigned height{10};
  unsigned mines_count{20};
  std::unique_ptr<Game> game{nullptr};

  void init_game();
  void draw_game(sf::RenderWindow &) const;
  void handle_opening_mine_event(sf::Event &, sf::RenderWindow &);

  const DrawingInfo get_drawing_info(sf::RenderWindow &) const;
};

#endif
