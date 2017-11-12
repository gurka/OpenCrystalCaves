#ifndef ENEMY_H_
#define ENEMY_H_

#include <utility>

#include "geometry.h"
#include "misc.h"

struct Enemy
{
  Enemy(geometry::Position position, int health)
    : position(std::move(position)),
      health(health)
  {
  }

  geometry::Position position;
  int health;

  static constexpr auto sprites = misc::make_array(68, 69, 70, 71);
};

#endif  // ENEMY_H_
