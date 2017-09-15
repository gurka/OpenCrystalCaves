#ifndef LEVEL_H_
#define LEVEL_H_

#include <vector>

#include "item.h"
#include "geometry.h"
#include "moving_platform.h"

class Object;

struct Background
{
  Background(int sprite_id, geometry::Size size_in_tiles)
    : sprite_id(sprite_id),
      size_in_tiles(size_in_tiles)
  {
  }

  int sprite_id;
  geometry::Size size_in_tiles;
};

enum class LevelId
{
  MAIN_LEVEL,
  LEVEL_ONE,
};

class Level
{
 public:
  virtual ~Level() = default;

  // The only functions that need to be declared here
  // are functions that are used for rendering the level
  virtual LevelId get_level_id() const = 0;

  // Width and height of the level in tiles
  virtual int get_tile_width() const = 0;
  virtual int get_tile_height() const = 0;

  virtual const Background& get_background() const = 0;
  virtual Item::Id get_tile_foreground(int tile_x, int tile_y) const = 0;
  virtual Item::Id get_tile_score(int tile_x, int tile_y) const = 0;
  virtual const std::vector<MovingPlatform>& get_moving_platforms() const = 0;
};

#endif  // LEVEL_H_
