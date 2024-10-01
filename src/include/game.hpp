#ifndef GAME_H
#define GAME_H

#include <memory>
#include <vector>
#include <functional>

#include "cell.hpp"

class Game
{
public:
  // enums
  enum class State : int
  {
    NOT_STARTED,
    IN_PROGRESS,
    DEFEATED,
    WINNED,
  };

  enum class LayoutCell
  {
    NOT_HAS_MINE,
    HAS_MINE,
    GUARANTEE_NOT_MINE,
  };

  // types
  using Cells = std::vector<Cell>;
  using Layout = std::vector<LayoutCell>;
  using WalkAroundCallback = std::function<void(unsigned x, unsigned y)>;

  // lifecycle
  explicit Game(unsigned width = 10, unsigned height = 10, unsigned mines_count = 20);
  virtual ~Game() = default;

  // public methods
  void open_cell(unsigned x, unsigned y);
  void toggle_cell_flag(unsigned x, unsigned y);

  const Cells &get_cells() const;
  State get_state() const;

private:
  // params
  unsigned width;
  unsigned height;
  unsigned mines_count;
  unsigned correct_flags{0};
  unsigned opened_cells{0};
  State state{State::NOT_STARTED};

  std::unique_ptr<Cells> create_empty_cells() const;
  std::unique_ptr<Cells> cells{create_empty_cells()};

  // initialize cells
  void init_cells(unsigned x, unsigned y);
  std::unique_ptr<Layout> create_layout() const;
  unsigned set_cells_with_guarantee(unsigned x, unsigned y, Layout &) const;
  void fill_layout_randomly(Layout &) const;
  std::unique_ptr<Cells> create_cells(Layout &) const;
  unsigned count_closest_mines(unsigned x, unsigned y, Layout &) const;

  // main methods
  void inner_open_cell(unsigned x, unsigned y);
  void open_cells_recursive(unsigned x, unsigned y);
  void open_all_cells();

  // helpers
  void walk_around_cell(unsigned x, unsigned y, WalkAroundCallback callback) const;
  Cell &get_cell(unsigned x, unsigned y);
  void check_game_winning();
};

#endif
