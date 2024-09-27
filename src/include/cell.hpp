#ifndef CELL_H
#define CELL_H

class Cell
{
public:
  enum class State : int
  {
    NOT_OPENED,
    WITH_FLAG,
    OPENED,
  };

  Cell(bool has_mine = false, unsigned closest_mines_count = 0);
  virtual ~Cell() = default;

  void open();
  void toggle_flag();

  bool get_has_mine() const;
  unsigned get_closest_mines_count() const;
  State get_state() const;

private:
  bool has_mine;
  unsigned closest_mines_count;
  State state{State::NOT_OPENED};
};

#endif
