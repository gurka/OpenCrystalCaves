#ifndef LEVEL_H_
#define LEVEL_H_

#include <vector>

#include "item.h"
#include "geometry.h"

class Object;

struct Background
{
  int sprite_id;
  geometry::Size size_in_tiles;
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
  virtual const Background& get_background() const = 0;
  virtual Item::Id get_tile_foreground(int tile_x, int tile_y) const = 0;
  virtual Item::Id get_tile_score(int tile_x, int tile_y) const = 0;

  // Objects
  virtual const std::vector<Object>& get_objects() const = 0;
};

#endif  // LEVEL_H_
