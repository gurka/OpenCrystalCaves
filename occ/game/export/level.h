#ifndef LEVEL_H_
#define LEVEL_H_

#include <vector>

#include "item.h"
#include "geometry.h"

class Player;

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


class Level
{
 public:
  virtual ~Level() = default;

  virtual void update(unsigned game_tick) = 0;

  virtual int get_tile_width() const = 0;
  virtual int get_tile_height() const = 0;

  virtual Player& get_player() = 0;
  virtual const Player& get_player() const = 0;

  virtual Item::Id get_tile_background(int tile_x, int tile_y) const = 0;
  virtual Item::Id get_tile_foreground(int tile_x, int tile_y) const = 0;

  virtual const std::vector<geometry::Rectangle>& get_aabbs() const = 0;
  virtual const std::vector<geometry::Position>& get_platforms() const = 0;

  // Dynamic things in the level
  virtual const std::vector<Object>& get_objects() const = 0;
  virtual const std::vector<geometry::Position>& get_moving_platforms() const = 0;

  // Helpers
  virtual bool collides(const geometry::Rectangle& rect) = 0;
};

#endif  // LEVEL_H_
