#ifndef GAME_PAGE_H
#define GAME_PAGE_H

#include <SFML/Graphics.hpp>

#include "route.hpp"

class GameRoute : public Route
{
public:
  ~GameRoute() override = default;

  void handle_event(sf::Event &, Router &) override;
  void update(float delta_time);
  void render(sf::RenderWindow &);
};

#endif
