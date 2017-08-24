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

  static constexpr int sprite_standing_right = 244;
  static constexpr std::array<int, 12> sprite_walking_right =
  {
      244, 245, 246, 247, 248, 249, 250, 251, 252, 253, 254, 255
  };
  static constexpr int sprite_jumping_right = 268;

  static constexpr int sprite_standing_left = 256;
  static constexpr std::array<int, 12> sprite_walking_left =
  {
      256, 257, 258, 259, 260, 261, 262, 263, 264, 265, 266, 267
  };
  static constexpr int sprite_jumping_left = 269;
};

#endif  // PLAYER_H_
