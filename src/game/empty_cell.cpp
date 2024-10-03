#include "empty_cell.hpp"

EmptyCell::FutureState EmptyCell::get_future_state() const { return future_state; }

void EmptyCell::set_future_state(EmptyCell::FutureState future_state)
{
  this->future_state = future_state;
}
