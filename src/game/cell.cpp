#include "cell.hpp"

Cell::Cell(bool has_mine, unsigned closest_mines_count)
    : has_mine{has_mine}, closest_mines_count{closest_mines_count} {}

void Cell::open()
{
  if (state == State::NOT_OPENED)
    state = State::OPENED;
}

void Cell::toggle_flag()
{
  switch (state)
  {
  case State::NOT_OPENED:
    state = State::WITH_FLAG;
    break;

  case State::WITH_FLAG:
    state = State::NOT_OPENED;
    break;

  default:
    break;
  }
}

Cell::State Cell::get_state() const { return state; }

bool Cell::get_has_mine() const { return has_mine; }

unsigned Cell::get_closest_mines_count() const { return closest_mines_count; }
