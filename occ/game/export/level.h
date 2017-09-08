#ifndef LEVEL_H_
#define LEVEL_H_

#include <vector>

#include "item.h"
#include "geometry.h"

struct Object
{
  Object(geometry::Position position, geometry::Size size, int sprite_id)
    : position(position),
      size(size),
      sprite_id(sprite_id)
  {
  }

  geometry::Position position;
  geometry::Size size;
  int sprite_id;
};

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
  const int num_sprites;  // >1 if animated

 private:
  static Vector<int> get_velocity(const geometry::Position& start,
                                  const geometry::Position& end,
                                  int speed)
  {
    // Hmm...
    return Vector<int>(start.x() == end.x() ? 0 : (start.x() < end.x() ? (1 * speed) : (-1 * speed)),
                       start.y() == end.y() ? 0 : (start.y() < end.y() ? (1 * speed) : (-1 * speed)));
  }
};

class Level
{
 public:
  virtual ~Level() = default;

  // The only functions that need to be declared here
  // are functions that are used for rendering the level

  // Width and height of the level in tiles
  virtual int get_tile_width() const = 0;
  virtual int get_tile_height() const = 0;

  // Static tiles
  virtual Item::Id get_tile_background(int tile_x, int tile_y) const = 0;
  virtual Item::Id get_tile_foreground(int tile_x, int tile_y) const = 0;

  // Dynamic tiles
  virtual const std::vector<MovingPlatform>& get_moving_platforms() const = 0;

  // Additional objects that should be rendered
  virtual const std::vector<Object>& get_objects() const = 0;
};

#endif  // LEVEL_H_
