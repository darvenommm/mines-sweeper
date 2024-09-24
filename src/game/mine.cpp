#include "mine.hpp"

Mine::Mine(bool has_mine, unsigned closest_mines_count)
    : has_mine{has_mine}, closest_mines_count{closest_mines_count}, status{Status::NOT_OPENED} {}

void Mine::open()
{
  if (status == Status::NOT_OPENED)
    status = has_mine ? Status::WITH_BOMB : Status::WITH_BOMB;
}

void Mine::toggle_flag()
{
  switch (status)
  {
  case Status::NOT_OPENED:
    status = Status::WITH_FLAG;
    break;

  case Status::WITH_FLAG:
    status = Status::NOT_OPENED;
    break;

  default:
    break;
  }
}

Mine::Status Mine::get_status() const { return status; }

unsigned Mine::get_closest_mines_count() const { return closest_mines_count; }
