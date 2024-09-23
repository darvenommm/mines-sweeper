#ifndef MENU_PAGE_H
#define MENU_PAGE_H

#include <SFML/Graphics.hpp>

#include "route.hpp"

class MenuRoute : public Route
{
public:
  ~MenuRoute() override = default;

  void handle_event(sf::Event &, Router &) override;
  void update(float delta_time);
  void render(sf::RenderWindow &);
};

#endif
