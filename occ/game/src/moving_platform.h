#pragma once

#include "geometry.h"
#include "vector.h"

struct MovingPlatform
{
  MovingPlatform(geometry::Position position, const bool horizontal);

  bool is_reverse() const { return velocity.x() > 0 || velocity.y() > 0; }

  geometry::Position position;
  Vector<int> velocity;
  const int sprite_id;
  const int num_sprites;  // >1 is animated
  geometry::Size collide_size = geometry::Size(16, 32);

  geometry::Position collide_position() const { return position + geometry::Position(0, -16); }
};
