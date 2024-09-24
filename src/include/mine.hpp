#ifndef MINE_H
#define MINE_H

class Mine
{
public:
  enum class Status : int
  {
    NOT_OPENED,
    WITH_FLAG,
    WITH_BOMB,
    WITHOUT_BOMB,
  };

  Mine(bool has_mine, unsigned closest_mines_count);
  virtual ~Mine() = default;

  void open();
  void toggle_flag();
  unsigned get_closest_mines_count() const;
  Status get_status() const;

private:
  bool has_mine{false};
  unsigned closest_mines_count{0};
  Status status{Status::NOT_OPENED};
};

#endif
