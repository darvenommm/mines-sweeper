#ifndef CELL_H
#define CELL_H

#include "base_cell.hpp"

class Cell : public BaseCell
{
private:
  unsigned MAX_CLOSEST_MINES_COUNT{8};

public:
  explicit Cell(unsigned mines_count);
  ~Cell() override = default;

  unsigned get_mines_count() const;

private:
  unsigned mines_count;
  void set_mines_count(unsigned mines_count);
};

#endif
