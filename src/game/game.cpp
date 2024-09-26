#include <random>
#include <ctime>

#include "game.hpp"

Game::Game(unsigned width, unsigned height, unsigned mines_count)
    : width{width}, height{height}, mines_count{mines_count}
{
  if (width * height >= mines_count)
    mines_count = width * height - 1;
}

void Game::open_mine(unsigned x, unsigned y)
{
  if (status == Status::NOT_STARTED)
  {
    set_mines(x, y);
    status = Status::STARTED;
  }

  (*mines)[y * width + x].open();
}

void Game::add_flag(unsigned, unsigned) {}

Game::Status Game::get_status() const { return status; }

const Game::Mines &Game::get_mines() const { return *mines; }

std::unique_ptr<Game::Mines> Game::init_start_mines()
{
  auto empty_mines{std::make_unique<Game::Mines>()};

  for (unsigned i{0}; i < height; ++i)
    for (unsigned j{0}; j < width; ++j)
      empty_mines->push_back(Mine{});

  return empty_mines;
}

void Game::set_mines(unsigned x, unsigned y)
{
  set_layout(x, y);
  set_final_mines();
}

void Game::set_layout(unsigned x, unsigned y)
{
  std::srand(std::time(nullptr));

  auto layout{std::make_unique<Game::Layout>()};

  float count_of_mines{static_cast<float>(mines_count)};
  float rest{static_cast<float>(width * height) - 1};

  for (unsigned i{0}; i < height; ++i)
    for (unsigned j{0}; j < width; ++j, --rest)
    {
      if (x == j && y == i)
      {
        layout->push_back(Mine{false});
        continue;
      }

      float real_change{count_of_mines / rest};
      float random{static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX)};
      bool has_mine{real_change > random};

      layout->push_back(Mine{has_mine});

      if (has_mine)
        --count_of_mines;
    }

  mines = std::move(layout);
}

void Game::set_final_mines()
{
  auto final_mines{std::make_unique<Game::Mines>()};

  for (unsigned i{0}; i < height; ++i)
    for (unsigned j{0}; j < width; ++j)
    {
      bool has_mine{(*mines)[i * width + j].get_has_mine()};
      unsigned closest_mines_count = count_closest_mines(j, i);

      final_mines->push_back(Mine{has_mine, closest_mines_count});
    }

  mines = std::move(final_mines);
}

unsigned Game::count_closest_mines(unsigned x, unsigned y) const
{
  unsigned result_count{0};

  std::function<void()> callback_if_has_closest_mine{
      [&result_count]
      { ++result_count; }};
  walk_around_cell(x, y, callback_if_has_closest_mine);

  return result_count;
}

void Game::walk_around_cell(unsigned x, unsigned y, std::function<void()> callback_if_mine) const
{
  unsigned start_x{static_cast<unsigned>(std::max<int>(static_cast<int>(x) - 1, 0))};
  unsigned end_x{std::min<unsigned>(x + 1, width - 1)};

  unsigned start_y{static_cast<unsigned>(std::max<int>(static_cast<int>(y) - 1, 0))};
  unsigned end_y{std::min<unsigned>(y + 1, height - 1)};

  for (unsigned i{start_y}; i <= end_y; ++i)
    for (unsigned j{start_x}; j <= end_x; ++j)
    {
      if (y == i && x == j)
        continue;

      if ((*mines)[i * width + j].get_has_mine())
        callback_if_mine();
    }
}
