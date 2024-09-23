#include <stdlib.h>

#include <memory>

#include "engine.hpp"

int main()
{
  std::make_unique<Engine>()->run();

  return EXIT_SUCCESS;
}
