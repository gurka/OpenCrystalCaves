#ifndef LEVEL_H_
#define LEVEL_H_

#include <array>
#include <vector>

#include "item.h"
#include "geometry.h"

class Level
{
 public:
  Level();
  Level(int width,
        int height,
        std::vector<Item::Id> tiles_background,
        std::vector<Item::Id> tiles_foreground,
        std::vector<geometry::Rectangle> aabbs,
        std::vector<geometry::Position> platforms);

  bool valid() const { return valid_; }

  Item::Id get_tile_background(int tile_x, int tile_y) const;
  Item::Id get_tile_foreground(int tile_x, int tile_y) const;

  const std::vector<geometry::Rectangle>& get_aabbs() const { return aabbs_; }
  const std::vector<geometry::Position>& get_platforms() const { return platforms_; }

  int get_tile_width() const { return width_; }
  int get_tile_height() const { return height_; }

 private:
  Item::Id get_tile(int tile_x, int tile_y, const std::vector<Item::Id>& items) const;

  int pos2index(int x, int y) const
  {
    return (y * width_) + x;
  }

  // false if Level is default constructed
  bool valid_;

  // Size of level in number of tiles
  int width_;
  int height_;

  // Tiles in order (see pos2index)
  std::vector<Item::Id> tiles_background_;
  std::vector<Item::Id> tiles_foreground_;

  // Bounding boxes for collision
  std::vector<geometry::Rectangle> aabbs_;
  std::vector<geometry::Position> platforms_;
};

#endif  // LEVEL_H_
