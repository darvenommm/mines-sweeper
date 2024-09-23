#ifndef PAGE_H
#define PAGE_H

#include <SFML/Graphics.hpp>

class Router;

class Route
{
public:
  virtual ~Route() = default;

  virtual void handle_event(sf::Event &, Router &, sf::RenderWindow &) = 0;
  virtual void update(float delta_time) = 0;
  virtual void render(sf::RenderWindow &) = 0;
};

#endif
