#include <utility>

#include "level.h"

Level::Level()
  : valid_(false),
    width_(0),
    height_(0)
{
}

Level::Level(int width,
             int height,
             std::vector<Item::Id> tiles_background,
             std::vector<Item::Id> tiles_middleground,
             std::vector<Item::Id> tiles_foreground,
             std::vector<geometry::Rectangle> aabbs)
  : valid_(true),
    width_(width),
    height_(height),
    tiles_background_(std::move(tiles_background)),
    tiles_middleground_(std::move(tiles_middleground)),
    tiles_foreground_(std::move(tiles_foreground)),
    aabbs_(std::move(aabbs))
{
}

Item::Id Level::get_tile_background(int tile_x, int tile_y) const
{
  return get_tile(tile_x, tile_y, tiles_background_);
}

Item::Id Level::get_tile_middleground(int tile_x, int tile_y) const
{
  return get_tile(tile_x, tile_y, tiles_middleground_);
}

Item::Id Level::get_tile_foreground(int tile_x, int tile_y) const
{
  return get_tile(tile_x, tile_y, tiles_foreground_);
}

Item::Id Level::get_tile(int tile_x, int tile_y, const std::vector<Item::Id>& items) const
{
  if (tile_x >= 0 && tile_x < width_ && tile_y >= 0 && tile_y < height_)
  {
    return items[pos2index(tile_x, tile_y)];
  }
  else
  {
    return Item::invalid;
  }
}
