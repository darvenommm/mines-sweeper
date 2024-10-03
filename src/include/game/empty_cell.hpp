#ifndef EMPTY_CELL_H
#define EMPTY_CELL_H

#include "base_cell.hpp"

class EmptyCell : public BaseCell
{
public:
  enum class FutureState : int
  {
    UNKNOWN,
    NOT_HAS_MINE,
    HAS_MINE,
  };

  ~EmptyCell() override = default;

  FutureState get_future_state() const;
  void set_future_state(FutureState);

private:
  FutureState future_state{FutureState::UNKNOWN};
};

#endif
