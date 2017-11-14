#ifndef MOVING_PLATFORM_H_
#define MOVING_PLATFORM_H_

#include "geometry.h"
#include "vector.h"

struct MovingPlatform
{
  MovingPlatform(geometry::Position position_start,
                 geometry::Position position_end,
                 int speed,
                 int sprite_id,
                 int num_sprites)
    : position(position_start),
      forward(true),
      position_start(position_start),
      position_end(position_end),
      velocity_forward(get_velocity(position_start, position_end, speed)),
      sprite_id(sprite_id),
      num_sprites(num_sprites)
  {
  }


  geometry::Position position;
  bool forward;  // true if going from start to end, otherwise false

  const geometry::Position position_start;
  const geometry::Position position_end;
  const Vector<int> velocity_forward;  // reversed if forward is false
  const int sprite_id;
  const int num_sprites;  // >1 is animated

 private:
  static Vector<int> get_velocity(const geometry::Position& start,
                                  const geometry::Position& end,
                                  int speed)
  {
    // Hmm...
    return Vector<int>(start.x() == end.x() ? 0 : (start.x() < end.x() ? speed : -speed),
                       start.y() == end.y() ? 0 : (start.y() < end.y() ? speed : -speed));
  }
};

#endif  // MOVING_PLATFORM_H_
