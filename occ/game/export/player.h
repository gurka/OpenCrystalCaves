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

  static constexpr int sprite_standing_right = 260;
  static constexpr std::array<int, 12> sprite_walking_right =
  {
      260, 261, 262, 263, 264, 265, 266, 267, 268, 269, 270, 271
  };
  static constexpr int sprite_jumping_right = 284;

  static constexpr int sprite_standing_left = 272;
  static constexpr std::array<int, 12> sprite_walking_left =
  {
      272, 273, 274, 275, 276, 277, 278, 279, 280, 281, 282, 283
  };
  static constexpr int sprite_jumping_left = 285;
};

#endif  // PLAYER_H_
