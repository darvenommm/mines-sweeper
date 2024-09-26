#ifndef GAME_H
#define GAME_H

#include <memory>
#include <vector>
#include <functional>

#include "mine.hpp"

class Game
{
public:
  using Mines = std::vector<Mine>;
  using Layout = Mines;

  enum class Status : int
  {
    NOT_STARTED,
    STARTED,
    DEFEATED,
    WINNED,
  };

  Game(unsigned width, unsigned height, unsigned mines_count);
  virtual ~Game() = default;

  void open_mine(unsigned x, unsigned y);
  void add_flag(unsigned x, unsigned y);

  const Mines &get_mines() const;
  Status get_status() const;

private:
  unsigned width{10};
  unsigned height{10};
  unsigned mines_count{20};
  Status status{Status::NOT_STARTED};

  std::unique_ptr<Mines> init_start_mines();
  std::unique_ptr<Mines> mines{init_start_mines()};

  // x and y it's the coordinates of a start user opened mine ;)
  void set_mines(unsigned x, unsigned y);
  void set_layout(unsigned x, unsigned y);
  unsigned count_closest_mines(unsigned x, unsigned y) const;
  void set_final_mines();

  void walk_around_cell(unsigned x, unsigned y, std::function<void()> callback_if_has_closest_mine) const;
};

#endif
