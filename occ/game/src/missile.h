#pragma once

#include "geometry.h"
#include "misc.h"

struct Missile
{
  Missile() : alive(false), frame(0), position(), right(false) {}

  bool alive;
  unsigned frame;
  geometry::Position position;
  bool right;             // Direction...
  unsigned cooldown = 0;  // cooldown from previous missile explosion

  bool is_in_cooldown() const;
  void set_cooldown();
  void update();

  // Note: this is the actual size of the missile and is used for collision detection.
  //       The missile sprites are size 16x16.
  static constexpr auto size = geometry::Size(16, 11);
  static constexpr auto speed = misc::make_array(4, 4, 4, 4, 4, 4, 6, 8, 10, 12, 14);
};
