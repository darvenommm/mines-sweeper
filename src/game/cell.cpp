#include <algorithm>

#include "cell.hpp"

Cell::Cell(unsigned mines_count) : BaseCell()
{
  set_mines_count(mines_count);
}

void Cell::set_mines_count(unsigned mines_count)
{
  this->mines_count = std::min(mines_count, MAX_CLOSEST_MINES_COUNT);
}

unsigned Cell::get_mines_count() const { return mines_count; }
