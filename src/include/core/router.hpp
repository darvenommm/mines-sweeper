#ifndef ROUTER_H
#define ROUTER_H

#include <memory>

#include "route.hpp"

class Router
{
public:
  Router(std::unique_ptr<Route>);

  virtual ~Router() = default;

  Route &get_route() const;
  void change_route(std::unique_ptr<Route>);

private:
  std::unique_ptr<Route> current_route{nullptr};
};

#endif
