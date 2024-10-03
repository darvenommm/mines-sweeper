#ifndef BASE_CELL_H
#define BASE_CELL_H

class CellsField;

class BaseCell
{
public:
  enum class State : int
  {
    NOT_OPENED,
    WITH_FLAG,
    OPENED,
  };

  virtual ~BaseCell() = default;

  void open();
  void toggle_flag();
  void force_open();

  State get_state() const;

protected:
  State state{State::NOT_OPENED};
};

#endif
