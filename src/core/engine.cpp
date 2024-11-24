#include <vector>

#include "engine.hpp"
#include "menu_route.hpp"

Engine::Engine()
{
  init_window();
  init_router();
  init_event();
}

void Engine::run() const
{
  auto clock{std::make_unique<sf::Clock>()};

  while (window->isOpen())
  {
    while (window->pollEvent(*event))
    {
      if (event->type == sf::Event::Closed)
        window->close();

      router->get_route().handle_event(*event, *router, *window);
    }

    router->get_route().update(clock->restart().asSeconds());
    router->get_route().render(*window);

    sf::sleep(sf::milliseconds(50));
  }
}

void Engine::init_window()
{
  if (window)
    return;

  auto new_window{std::make_unique<sf::RenderWindow>(sf::VideoMode::getDesktopMode(), L"Mine Sweeper")};
  window.reset(new_window.release());
}

void Engine::init_router()
{
  if (router)
    return;

  auto new_router{std::make_unique<Router>(std::make_unique<MenuRoute>())};
  router.reset(new_router.release());
}

void Engine::init_event()
{
  if (event)
    return;

  event.reset(std::make_unique<sf::Event>().release());
}
