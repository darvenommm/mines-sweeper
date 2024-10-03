#ifndef GAME_H
#define GAME_H

#include <memory>

#include "cells_field.hpp"
#include "base_cell.hpp"

class Game
{
public:
  enum class State : int
  {
    NOT_STARTED,
    IN_PROGRESS,
    DEFEATED,
    WINNED,
  };

  explicit Game(std::unique_ptr<CellsField> cells_field);

  void open(unsigned x, unsigned y);
  void toggle_flag(unsigned x, unsigned y);

  const BaseCell *get_cell(unsigned x, unsigned y) const;
  const CellsField::Cells &get_cells() const;
  State get_state() const;

private:
  std::unique_ptr<CellsField> cells_field{nullptr};
  unsigned opened_cells_count{0};
  State state{State::NOT_STARTED};

  void open_cell(unsigned x, unsigned y);
  void open_recursive(unsigned x, unsigned y);
  void open_with_flags(unsigned x, unsigned y);

  void defeat();
  void win();

  bool check_is_winned();
};

#endif
