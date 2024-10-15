#pragma once

#include "geometry.h"
#include "vector.h"

struct Level;

struct MovingPlatform
{
  MovingPlatform(geometry::Position position, const bool horizontal, const bool controlled);

  void update(const Level& level);
  bool is_reverse() const { return velocity.x() > 0 || velocity.y() > 0; }

  geometry::Position position;
  const int sprite_id;
  const int num_sprites;  // >1 is animated
  geometry::Size collide_size = geometry::Size(16, 32);

  Vector<int> get_velocity(const Level& level) const;

 private:
  bool is_moving(const Level& level) const;
  geometry::Position collide_position() const { return position + geometry::Position(0, -16); }
  Vector<int> velocity;
  bool controlled_;
};
