#include "base_cell.hpp"

void BaseCell::open()
{
  switch (state)
  {
  case State::NOT_OPENED:
    state = State::OPENED;
    break;

  case State::WITH_FLAG:
  case State::OPENED:
    break;
  }
}

void BaseCell::toggle_flag()
{
  switch (state)
  {
  case State::NOT_OPENED:
    state = State::WITH_FLAG;
    break;

  case State::WITH_FLAG:
    state = State::NOT_OPENED;
    break;

  case State::OPENED:
    break;
  }
}

void BaseCell::force_open() { state = State::OPENED; }

BaseCell::State BaseCell::get_state() const { return state; }
