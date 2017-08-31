#ifndef PLAYER_H_
#define PLAYER_H_

#include <array>

#include "geometry.h"
#include "vector.h"

struct Player
{
  static constexpr geometry::Size size = geometry::Size(12, 16);

  /**
   * Player velocity
   */
  Vector<int> velocity = Vector<int>();

  /**
   * Player position
   */
  geometry::Position position = geometry::Position();

  /**
   * Player information
   */
  bool collide_x = false;
  bool collide_y = false;

  enum class Direction
  {
    right,
    left
  } direction = Direction::right;

  bool walking = false;
  unsigned walk_tick = 0u;

  bool jumping = false;
  unsigned jump_tick = 0u;

  bool falling = false;

  bool shooting = false;
};

#endif  // PLAYER_H_
