#ifndef LEVEL_H_
#define LEVEL_H_

#include <vector>

#include "item.h"
#include "geometry.h"

struct Object
{
  Object(geometry::Position position, geometry::Size size, int sprite_id, int num_sprites)
    : position(position),
      size(size),
      sprite_id(sprite_id),
      num_sprites(num_sprites)
  {
  }

  geometry::Position position;
  geometry::Size size;
  int sprite_id;
  int num_sprites;  // >1 is animated
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

  // Objects
  virtual const std::vector<Object>& get_objects() const = 0;
};

#endif  // LEVEL_H_
