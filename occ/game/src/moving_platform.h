#pragma once

#include "geometry.h"
#include "vector.h"

struct Level;

struct MovingPlatform
{
  MovingPlatform(geometry::Position position, const bool horizontal, const bool moving);

  void update(const Level& level);
  bool is_reverse() const { return velocity.x() > 0 || velocity.y() > 0; }

  geometry::Position position;
  bool moving;
  const int sprite_id;
  const int num_sprites;  // >1 is animated
  geometry::Size collide_size = geometry::Size(16, 32);

  Vector<int> get_velocity() const { return moving ? velocity : Vector<int>(0, 0); }

 private:
  geometry::Position collide_position() const { return position + geometry::Position(0, -16); }
  Vector<int> velocity;
};
