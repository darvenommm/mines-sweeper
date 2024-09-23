#include "router.hpp"

Router::Router(std::unique_ptr<Route> route) : current_route{std::move(route)} {}

Route &Router::get_route() const { return *current_route; }

void Router::change_route(std::unique_ptr<Route> route) { current_route = std::move(route); };
