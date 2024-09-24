#ifndef GAME_PAGE_H
#define GAME_PAGE_H

#include <SFML/Graphics.hpp>

#include "route.hpp"
#include "game.hpp"

class GameRoute : public Route
{
public:
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
};

#endif
