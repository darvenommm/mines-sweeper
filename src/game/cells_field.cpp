#include <iostream>
#include <algorithm>
#include <random>

#include "cells_field.hpp"

CellsField::CellsField(unsigned width, unsigned height, unsigned desired_mines_count)
{
  set_width(width);
  set_height(height);
  square = this->width * this->height;

  set_desired_mines_count(desired_mines_count);
  init_empty_cells();
}

void CellsField::set_width(unsigned width)
{
  this->width = std::clamp(width, MIN_WIDTH, MAX_WIDTH);
}

void CellsField::set_height(unsigned height)
{
  this->height = std::clamp(height, MIN_HEIGHT, MAX_HEIGHT);
}

void CellsField::set_desired_mines_count(unsigned desired_mines_count)
{
  this->desired_mines_count = std::clamp(desired_mines_count, MIN_MINES_COUNT, square);
}

CellsField::Cells &CellsField::get_cells() const { return *cells; }

void CellsField::init_empty_cells()
{
  auto result_cells = std::make_unique<CellsField::Cells>();
  result_cells->reserve(square);

  for (unsigned i{0}; i < square; ++i)
    result_cells->emplace_back(std::make_unique<EmptyCell>());

  cells = std::move(result_cells);
}

void CellsField::init(unsigned x, unsigned y)
{
  if (state == State::INITIALIZED)
    return;
  else
    state = State::INITIALIZED;

  set_cells_guarantee_without_bomb(x, y);
  fill_randomly();
  fill_in_real_cells();
}

void CellsField::open_all()
{
  if (state == State::NOT_INITIALIZED || state == State::ALL_OPENED)
    return;
  else
    state = State::ALL_OPENED;

  for (auto &cell : *cells)
    cell->force_open();
}

unsigned CellsField::get_square() const { return square; }
unsigned CellsField::get_mines_count() const { return mines_count; }

void CellsField::set_cells_guarantee_without_bomb(unsigned x, unsigned y)
{
  guarantee_without_bomb_count = 1;
  EmptyCell &center_cell{get_empty_cell(x, y)};
  center_cell.set_future_state(EmptyCell::FutureState::NOT_HAS_MINE);

  WalkAroundCallback callback{
      [this](unsigned x, unsigned y)
      {
        EmptyCell &current_cell{get_empty_cell(x, y)};
        current_cell.set_future_state(EmptyCell::FutureState::NOT_HAS_MINE);
        ++guarantee_without_bomb_count;
      }};
  walk_around(x, y, callback);

  mines_count = std::clamp(desired_mines_count, MIN_MINES_COUNT, square - guarantee_without_bomb_count);
}

void CellsField::fill_randomly()
{
  using State = EmptyCell::FutureState;

  std::random_device rd;
  std::mt19937 gen{rd()};
  std::uniform_real_distribution<float> dis{0.0f, 1.0f};

  float mines_count{static_cast<float>(this->mines_count)};
  float rest{static_cast<float>(square - guarantee_without_bomb_count)};

  for (unsigned y{0}; y < height; ++y)
    for (unsigned x{0}; x < width; ++x, --rest)
    {
      EmptyCell &current_cell{get_empty_cell(x, y)};

      if (current_cell.get_future_state() == State::NOT_HAS_MINE)
      {
        ++rest;
        continue;
      }

      auto a = dis(gen);
      bool has_mine = (mines_count / rest) > a;

      current_cell.set_future_state(has_mine ? State::HAS_MINE : State::NOT_HAS_MINE);
      mines_count -= has_mine ? 1 : 0;
    }
}

void CellsField::fill_in_real_cells()
{
  const char *error_message{"Unknown or incorrect future state has cell! It's incorrect!"};

  auto filled_cells{std::make_unique<Cells>()};
  filled_cells->reserve(square);

  for (unsigned y{0}; y < height; ++y)
    for (unsigned x{0}; x < width; ++x)
    {
      EmptyCell &current_cell{get_empty_cell(x, y)};

      switch (current_cell.get_future_state())
      {
      case EmptyCell::FutureState::NOT_HAS_MINE:
      {
        unsigned closest_mines_count{calculate_closest_mines_count(x, y)};
        filled_cells->emplace_back(std::make_unique<Cell>(closest_mines_count));
        break;
      }

      case EmptyCell::FutureState::HAS_MINE:
        filled_cells->emplace_back(std::make_unique<Bomb>());
        break;

      case EmptyCell::FutureState::UNKNOWN:
      default:
        throw std::runtime_error(error_message);
      }
    }

  cells = std::move(filled_cells);
}

unsigned CellsField::calculate_closest_mines_count(unsigned x, unsigned y)
{
  unsigned closest_mines_count{0};

  WalkAroundCallback callback{
      [this, &closest_mines_count](unsigned x, unsigned y)
      {
        EmptyCell &current_cell{get_empty_cell(x, y)};

        if (current_cell.get_future_state() == EmptyCell::FutureState::HAS_MINE)
          ++closest_mines_count;
      }};
  walk_around(x, y, callback);

  return closest_mines_count;
}

BaseCell *CellsField::get_cell(unsigned x, unsigned y) const
{
  if (x >= width || y >= height)
    throw std::out_of_range("Cell coordinates are out of range");

  return (*cells)[y * width + x].get();
}

EmptyCell &CellsField::get_empty_cell(unsigned x, unsigned y) const
{
  EmptyCell *empty_cell_pointer = dynamic_cast<EmptyCell *>(get_cell(x, y));

  if (!empty_cell_pointer)
    throw std::runtime_error("Cell is not of type EmptyCell");

  return *empty_cell_pointer;
}

void CellsField::walk_around(unsigned x, unsigned y, CellsField::WalkAroundCallback callback) const
{
  int temp_x{static_cast<int>(x)};
  unsigned start_x{static_cast<unsigned>(std::max(temp_x - 1, 0))};
  unsigned end_x{std::min<unsigned>(x + 1, width - 1)};

  int temp_y{static_cast<int>(y)};
  unsigned start_y{static_cast<unsigned>(std::max(temp_y - 1, 0))};
  unsigned end_y{std::min<unsigned>(y + 1, height - 1)};

  for (unsigned current_y{start_y}; current_y <= end_y; ++current_y)
    for (unsigned current_x{start_x}; current_x <= end_x; ++current_x)
      if (current_y == y && current_x == x)
        continue;
      else
        callback(current_x, current_y);
}
