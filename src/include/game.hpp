#ifndef GAME_H
#define GAME_H

#include <memory>
#include <vector>

#include "mine.hpp"

class Game
{
public:
  using Layout = std::vector<bool>;
  using Mines = std::vector<Mine>;

  enum class Status : int
  {
    UNKNOWN,
    DEFEATED,
    WINNED,
  };

  Game(unsigned width = 10, unsigned height = 10, unsigned mines_count = 20);
  virtual ~Game() = default;

  void open_mine(unsigned x, unsigned y);
  void add_flag(unsigned x, unsigned y);

  const Mines &get_mines() const;
  Status get_status() const;

private:
  unsigned width{10};
  unsigned height{10};
  unsigned mines_count{20};
  Status status{Status::UNKNOWN};

  std::unique_ptr<Mines> mines{nullptr};

  // x and y it's the coordinates of a start user opened mine ;)
  void set_mines(unsigned x, unsigned y);
  std::unique_ptr<Layout> create_layout(unsigned x, unsigned y) const;
  std::unique_ptr<Mines> create_mines_from_layout(Layout &) const;
  unsigned count_closest_mines(unsigned x, unsigned y, Layout &) const;
};

#endif
