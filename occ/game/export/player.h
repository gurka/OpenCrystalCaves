#ifndef PLAYER_H_
#define PLAYER_H_

#include <array>

#include "geometry.h"
#include "vector.h"

struct Player
{
  geometry::Position position = geometry::Position();
  Vector<int> velocity = Vector<int>();

  enum class Direction
  {
    right,
    left
  } direction = Direction::right;

  enum class State
  {
    still,
    walking,
    jumping
  } state = State::still;

  unsigned animation_tick = 0u;
  unsigned jump_tick = 0u;
};

#endif  // PLAYER_H_
