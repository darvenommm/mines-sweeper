#include <random>
#include <ctime>
#include <queue>

#include <iostream>

#include "game.hpp"

Game::Game(unsigned width, unsigned height, unsigned mines_count)
    : width{width}, height{height}, mines_count{mines_count}
{
  if (mines_count > width * height)
    mines_count = width * height;
}

std::unique_ptr<Game::Cells> Game::create_empty_cells() const
{
  auto empty_mines{std::make_unique<Game::Cells>()};
  empty_mines->reserve(width * height);

  for (unsigned i{0}; i < height; ++i)
    for (unsigned j{0}; j < width; ++j)
      empty_mines->push_back(Cell{});

  return empty_mines;
}

// Need rewriting to several methods! It can be in future...
void Game::open_cell(unsigned x, unsigned y)
{
  switch (state)
  {
  case State::NOT_STARTED:
    state = State::IN_PROGRESS;
    init_cells(x, y);
    open_cells_recursive(x, y);
    break;

  case State::IN_PROGRESS:
  {
    Cell &current_cell{get_cell(x, y)};

    switch (current_cell.get_state())
    {
    case Cell::State::NOT_OPENED:
      if (current_cell.get_has_mine())
      {
        state = State::DEFEATED;
        open_all_cells();
        return;
      }

      if (current_cell.get_closest_mines_count() == 0)
        open_cells_recursive(x, y);
      else
        inner_open_cell(x, y);
      break;

    case Cell::State::WITH_FLAG:
      break;

    case Cell::State::OPENED:
      break;
    }
    break;
  }

  case State::DEFEATED:
  case State::WINNED:
    return;
  }

  check_game_winning();
}

void Game::toggle_cell_flag(unsigned x, unsigned y)
{
  if (state != Game::State::IN_PROGRESS)
    return;

  Cell &current_cell{get_cell(x, y)};

  current_cell.toggle_flag();

  if (current_cell.get_has_mine())
    correct_flags += (current_cell.get_state() == Cell::State::WITH_FLAG) ? 1 : -1;

  check_game_winning();
}

void Game::init_cells(unsigned x, unsigned y)
{
  std::unique_ptr<Game::Layout> layout{create_layout()};
  unsigned count_guarantee_cells{set_cells_with_guarantee(x, y, *layout)};
  unsigned max_mines_count{width * height - count_guarantee_cells};
  mines_count = mines_count > max_mines_count ? max_mines_count : mines_count;
  fill_layout_randomly(*layout);
  cells = create_cells(*layout);

  for (unsigned i{0}; i < height; ++i)
  {
    for (unsigned j{0}; j < width; ++j)
      std::cout << (*cells)[i * width + j].get_has_mine() << ' ';
    std::cout << std::endl;
  }
}

std::unique_ptr<Game::Layout> Game::create_layout() const
{
  std::unique_ptr<Game::Layout> layout{std::make_unique<Game::Layout>()};
  layout->resize(width * height, Game::LayoutCell::NOT_HAS_MINE);
  return layout;
}

unsigned Game::set_cells_with_guarantee(unsigned x, unsigned y, Layout &layout) const
{
  unsigned count_guarantee_not_mine{1};
  layout[y * width + x] = Game::LayoutCell::GUARANTEE_NOT_MINE;

  Game::WalkAroundCallback callback{
      [this, &layout, &count_guarantee_not_mine](unsigned current_x, unsigned current_y)
      {
        layout[current_y * width + current_x] = Game::LayoutCell::GUARANTEE_NOT_MINE;
        ++count_guarantee_not_mine;
      }};
  walk_around_cell(x, y, callback);

  return count_guarantee_not_mine;
}

void Game::fill_layout_randomly(Game::Layout &layout) const
{
  std::srand(std::time(nullptr));

  float count_of_mines{static_cast<float>(mines_count)};
  float rest{static_cast<float>(width * height)};

  for (unsigned i{0}; i < height; ++i)
    for (unsigned j{0}; j < width; ++j, --rest)
    {
      if (layout[i * width + j] == Game::LayoutCell::GUARANTEE_NOT_MINE)
      {
        layout[i * width + j] = Game::LayoutCell::NOT_HAS_MINE;
        continue;
      }

      float real_change{count_of_mines / rest};
      float random{static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX)};
      bool has_mine{real_change > random};

      layout[i * width + j] = has_mine ? Game::LayoutCell::HAS_MINE : Game::LayoutCell::NOT_HAS_MINE;

      if (has_mine)
        --count_of_mines;
    }
}

std::unique_ptr<Game::Cells> Game::create_cells(Layout &layout) const
{
  auto result_cells{std::make_unique<Game::Cells>()};
  result_cells->reserve(width * height);

  for (unsigned i{0}; i < height; ++i)
    for (unsigned j{0}; j < width; ++j)
    {
      bool has_mine{layout[i * width + j] == Game::LayoutCell::HAS_MINE};
      unsigned closest_mines_count = count_closest_mines(j, i, layout);

      result_cells->push_back(Cell{has_mine, closest_mines_count});
    }

  return result_cells;
}

unsigned Game::count_closest_mines(unsigned x, unsigned y, Game::Layout &layout) const
{
  unsigned result_count{0};

  Game::WalkAroundCallback callback{
      [this, &result_count, &layout](unsigned x, unsigned y)
      {
        if (layout[y * width + x] == Game::LayoutCell::HAS_MINE)
          ++result_count;
      }};
  walk_around_cell(x, y, callback);

  return result_count;
}

void Game::inner_open_cell(unsigned x, unsigned y)
{
  get_cell(x, y).open();
  ++opened_cells;
}

void Game::open_cells_recursive(unsigned x, unsigned y)
{
  struct CellInfo
  {
    Cell &cell;
    unsigned x;
    unsigned y;
  };

  auto cells_queue{std::make_unique<std::queue<CellInfo>>()};
  cells_queue->push(CellInfo{get_cell(x, y), x, y});

  while (!cells_queue->empty())
  {
    auto [cell, x, y] = cells_queue->front();
    cells_queue->pop();

    if (cell.get_state() == Cell::State::NOT_OPENED)
      inner_open_cell(x, y);

    if (cell.get_closest_mines_count() > 0)
      continue;

    Game::WalkAroundCallback callback{
        [this, &cells_queue](unsigned x, unsigned y)
        {
          Cell &current_cell{get_cell(x, y)};

          if (current_cell.get_state() == Cell::State::NOT_OPENED && !current_cell.get_has_mine())
            cells_queue->push(CellInfo{current_cell, x, y});
        }};
    walk_around_cell(x, y, callback);
  }
}

void Game::open_all_cells()
{
  for (unsigned i{0}; i < height; ++i)
    for (unsigned j{0}; j < width; ++j)
    {
      Cell &current_cell{(*cells)[i * width + j]};

      if (current_cell.get_state() == Cell::State::WITH_FLAG)
        current_cell.toggle_flag();

      current_cell.open();
    }
}

Game::State Game::get_state() const { return state; }

const Game::Cells &Game::get_cells() const { return *cells; }

void Game::walk_around_cell(unsigned x, unsigned y, Game::WalkAroundCallback callback) const
{
  int temp_x{static_cast<int>(x)};
  unsigned start_x{static_cast<unsigned>(std::max(temp_x - 1, 0))};
  unsigned end_x{std::min<unsigned>(x + 1, width - 1)};

  int temp_y{static_cast<int>(y)};
  unsigned start_y{static_cast<unsigned>(std::max(temp_y - 1, 0))};
  unsigned end_y{std::min<unsigned>(y + 1, height - 1)};

  for (unsigned i{start_y}; i <= end_y; ++i)
    for (unsigned j{start_x}; j <= end_x; ++j)
    {
      if (y == i && x == j)
        continue;
      else
        callback(j, i);
    }
}

Cell &Game::get_cell(unsigned x, unsigned y) { return (*cells)[y * width + x]; }

void Game::check_game_winning()
{
  if (correct_flags == mines_count || opened_cells == width * height - mines_count)
  {
    state = State::WINNED;
    open_all_cells();
  }
}
