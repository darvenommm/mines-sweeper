#ifndef CELLS_FIELD_H
#define CELLS_FIELD_H

#include <memory>
#include <vector>
#include <functional>

#include "base_cell.hpp"
#include "cell.hpp"
#include "bomb.hpp"
#include "empty_cell.hpp"

class CellsField
{
private:
  unsigned MIN_WIDTH{4};
  unsigned MAX_WIDTH{50};

  unsigned MIN_HEIGHT{4};
  unsigned MAX_HEIGHT{50};

  unsigned MIN_MINES_COUNT{1};

  enum class State : int
  {
    NOT_INITIALIZED,
    INITIALIZED,
    ALL_OPENED,
  };

public:
  using Cells = std::vector<std::unique_ptr<BaseCell>>;
  using WalkAroundCallback = std::function<void(unsigned x, unsigned y)>;

  explicit CellsField(unsigned width, unsigned height, unsigned desired_mines_count);

  void init(unsigned x, unsigned y);
  void open_all();

  Cells &get_cells() const;
  BaseCell *get_cell(unsigned x, unsigned y) const;
  unsigned get_square() const;
  unsigned get_mines_count() const;
  void walk_around(unsigned x, unsigned y, WalkAroundCallback) const;

private:
  unsigned width;
  unsigned height;
  unsigned square;
  void set_width(unsigned width);
  void set_height(unsigned height);

  unsigned desired_mines_count; // wish count
  unsigned mines_count;         // real count
  unsigned guarantee_without_bomb_count;
  void set_desired_mines_count(unsigned desired_mines_count);

  State state{State::NOT_INITIALIZED};

  std::unique_ptr<Cells> cells{nullptr};
  void init_empty_cells();

  // for initialization
  void set_cells_guarantee_without_bomb(unsigned x, unsigned y);
  void fill_randomly();
  void fill_in_real_cells();
  unsigned calculate_closest_mines_count(unsigned x, unsigned y);

  EmptyCell &get_empty_cell(unsigned x, unsigned y) const;
};

#endif
