#include <random>
#include <ctime>

#include "game.hpp"

Game::Game(unsigned width, unsigned height, unsigned mines_count)
    : width{width}, height{height}, mines_count{mines_count}, status{Status::UNKNOWN}
{
  if (width * height > mines_count)
    mines_count = width * height - 1;

  init_start_mines();
}

void Game::open_mine(unsigned x, unsigned y)
{
  if (!mines)
    set_mines(x, y);

  (*mines)[y * width + x].open();
}

void Game::add_flag(unsigned, unsigned) {}

Game::Status Game::get_status() const { return status; }

const Game::Mines &Game::get_mines() const { return *mines; }

void Game::init_start_mines()
{
  auto not_initialized_mines{std::make_unique<Game::Mines>()};

  for (unsigned i{0}; i < height; ++i)
    for (unsigned j{0}; j < width; ++j)
      not_initialized_mines->push_back(Mine{false, 0});

  mines = std::move(not_initialized_mines);
}

void Game::set_mines(unsigned x, unsigned y)
{
  auto layout{create_layout(x, y)};
  mines = create_mines_from_layout(*layout);
}

std::unique_ptr<Game::Layout> Game::create_layout(unsigned x, unsigned y) const
{
  std::srand(std::time(nullptr));

  auto layout{std::make_unique<Game::Layout>()};
  layout->resize(width * height);

  float count_of_mines{static_cast<float>(mines_count)};
  float rest{static_cast<float>(width * height)};

  for (unsigned i{0}; i < height; ++i)
    for (unsigned j{0}; j < width; ++j, --rest)
    {
      if (i == y && x == j)
      {
        (*layout)[i * width + j] = false;
        continue;
      }

      float real_change{count_of_mines / rest};
      float random{static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX)};

      (*layout)[i * width + j] = real_change > random;
    }

  return layout;
}

std::unique_ptr<Game::Mines> Game::create_mines_from_layout(Game::Layout &layout) const
{
  auto result_mines{std::make_unique<Game::Mines>()};

  for (unsigned i{0}; i < height; ++i)
    for (unsigned j{0}; j < width; ++j)
    {
      bool has_mine{layout[i * width + j]};
      unsigned closest_mines_count = count_closest_mines(j, i, layout);

      mines->push_back(Mine{has_mine, closest_mines_count});
    }

  return result_mines;
}

unsigned Game::count_closest_mines(unsigned x, unsigned y, Game::Layout &layout) const
{
  unsigned result_count{0};

  int start_x{std::max(static_cast<int>(x) - 1, 0)};
  int end_x{std::min(static_cast<int>(y) + 1, static_cast<int>(width))};

  int start_y{std::max(static_cast<int>(x) - 1, 0)};
  int end_y{std::min(static_cast<int>(y) + 1, static_cast<int>(height))};

  for (int i{start_y}; i < end_y; ++i)
    for (int j{start_x}; j < end_x; ++j)
      if (layout[i * width + j])
        ++result_count;

  return result_count;
}
