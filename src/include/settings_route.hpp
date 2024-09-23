#ifndef SETTINGS_PAGE_H
#define SETTINGS_PAGE_H

#include <SFML/Graphics.hpp>

#include "route.hpp"

class SettingsRoute : public Route
{
public:
  ~SettingsRoute() override = default;

  void handle_event(sf::Event &, Router &) override;
  void update(float delta_time);
  void render(sf::RenderWindow &);
};

#endif
