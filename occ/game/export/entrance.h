#pragma once

#include <utility>

#include "geometry.h"
#include "misc.h"

enum class EntranceState : int
{
  CLOSED = 0,
  OPEN = 1,
  COMPLETE = 2,
};

struct Entrance
{
  Entrance(geometry::Position position, int level, EntranceState state) : position(std::move(position)), level(level), state(state) {}

  geometry::Position position;
  int level;
  EntranceState state;
  int counter = 0;

  void update();
  int get_sprite() const;
};
