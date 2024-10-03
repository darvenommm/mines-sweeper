#include <queue>

#include "game.hpp"

Game::Game(std::unique_ptr<CellsField> cells_field) : cells_field{std::move(cells_field)} {}

const CellsField::Cells &Game::get_cells() const { return cells_field->get_cells(); }

Game::State Game::get_state() const { return state; }

const BaseCell *Game::get_cell(unsigned x, unsigned y) const { return cells_field->get_cell(x, y); }

void Game::open(unsigned x, unsigned y)
{
  switch (state)
  {
  case State::NOT_STARTED:
    cells_field->init(x, y);
    open_recursive(x, y);
    state = State::IN_PROGRESS;
    break;

  case State::IN_PROGRESS:
  {
    BaseCell *cell{cells_field->get_cell(x, y)};

    switch (cell->get_state())
    {
    case BaseCell::State::NOT_OPENED:
      if (dynamic_cast<Bomb *>(cell))
        return defeat();

      if (dynamic_cast<Cell *>(cell)->get_mines_count() == 0)
        open_recursive(x, y);
      else
        open_cell(x, y);
      break;

    case BaseCell::State::WITH_FLAG:
      break;

    case BaseCell::State::OPENED:
      open_with_flags(x, y);
      break;
    }
    break;
  }

  case State::DEFEATED:
  case State::WINNED:
    break;
  }

  if (check_is_winned())
    win();
}

void Game::toggle_flag(unsigned x, unsigned y)
{
  cells_field->get_cell(x, y)->toggle_flag();
}

void Game::open_cell(unsigned x, unsigned y)
{
  BaseCell *cell{cells_field->get_cell(x, y)};

  if (cell->get_state() == BaseCell::State::NOT_OPENED)
  {
    cell->open();
    ++opened_cells_count;
  }
}

void Game::defeat()
{
  state = State::DEFEATED;
  cells_field->open_all();
}

void Game::win()
{
  state = State::WINNED;
  cells_field->open_all();
}

bool Game::check_is_winned()
{
  return opened_cells_count == (cells_field->get_square() - cells_field->get_mines_count());
}

void Game::open_recursive(unsigned x, unsigned y)
{
  struct CellInfo
  {
    Cell *cell;
    unsigned x;
    unsigned y;
  };

  Cell *start_cell{dynamic_cast<Cell *>(cells_field->get_cell(x, y))};

  if (!start_cell)
    return;

  auto cells_queue{std::make_unique<std::queue<CellInfo>>()};
  cells_queue->push(CellInfo{start_cell, x, y});

  while (!cells_queue->empty())
  {
    auto [cell, x, y] = cells_queue->front();
    cells_queue->pop();

    if (cell->get_state() == Cell::State::NOT_OPENED)
      open_cell(x, y);

    if (cell->get_mines_count() > 0)
      continue;

    CellsField::WalkAroundCallback callback{
        [this, &cells_queue](unsigned x, unsigned y)
        {
          Cell *current_cell{dynamic_cast<Cell *>(cells_field->get_cell(x, y))};

          if (current_cell && current_cell->get_state() == Cell::State::NOT_OPENED)
            cells_queue->push(CellInfo{current_cell, x, y});
        }};
    cells_field->walk_around(x, y, callback);
  }
}

void Game::open_with_flags(unsigned x, unsigned y)
{
  Cell *cell{dynamic_cast<Cell *>(cells_field->get_cell(x, y))};

  if (!cell)
    return;

  if (cell->get_state() == Cell::State::OPENED && cell->get_mines_count() > 0)
  {
    unsigned flags_count{0};

    CellsField::WalkAroundCallback flag_callback{
        [this, &flags_count](unsigned x, unsigned y)
        {
          if (cells_field->get_cell(x, y)->get_state() == Cell::State::WITH_FLAG)
            ++flags_count;
        }};
    cells_field->walk_around(x, y, flag_callback);

    if (cell->get_mines_count() != flags_count)
      return;

    CellsField::WalkAroundCallback opening_callback{
        [this](unsigned x, unsigned y)
        {
          BaseCell *current_cell{cells_field->get_cell(x, y)};

          if (current_cell->get_state() == BaseCell::State::WITH_FLAG)
            return;

          if (dynamic_cast<Bomb *>(current_cell))
            defeat();
          else
            open_cell(x, y);
        }};
    cells_field->walk_around(x, y, opening_callback);
  }
}
