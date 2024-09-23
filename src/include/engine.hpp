#ifndef ENGINE_H
#define ENGINE_H

#include <memory>
#include <SFML/Graphics.hpp>

#include "router.hpp"

class Engine
{
public:
  Engine();
  virtual ~Engine() = default;

  void run() const;

private:
  std::unique_ptr<sf::RenderWindow> window{nullptr};
  std::unique_ptr<Router> router{nullptr};
  std::unique_ptr<sf::Event> event{nullptr};

  void init_window();
  void init_router();
  void init_event();
};

#endif
