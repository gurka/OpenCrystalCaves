#ifndef EXPLOSION_H_
#define EXPLOSION_H_

#include "geometry.h"
#include "misc.h"

struct Explosion
{
  Explosion()
    : alive(false),
      frame(0),
      position()
  {
  }

  bool alive;
  unsigned frame;
  geometry::Position position;

  static constexpr auto sprites = misc::make_array(28, 29, 30, 31, 30, 29, 28);
};

#endif  // EXPLOSION_H_
