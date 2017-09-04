#include "level_base.h"

#include <utility>

#include "geometry.h"

LevelBase::LevelBase(int width,
                     int height,
                     std::vector<Item::Id> tiles_background,
                     std::vector<Item::Id> tiles_foreground,
                     std::vector<geometry::Rectangle> aabbs,
                     std::vector<geometry::Position> platforms)
  : width_(width),
    height_(height),
    player_(),
    tiles_background_(std::move(tiles_background)),
    tiles_foreground_(std::move(tiles_foreground)),
    aabbs_(std::move(aabbs)),
    platforms_(std::move(platforms)),
    objects_(),
    moving_platforms_()
{
}

Item::Id LevelBase::get_tile_background(int tile_x, int tile_y) const
{
  return get_tile(tile_x, tile_y, tiles_background_);
}

Item::Id LevelBase::get_tile_foreground(int tile_x, int tile_y) const
{
  return get_tile(tile_x, tile_y, tiles_foreground_);
}

Item::Id LevelBase::get_tile(int tile_x, int tile_y, const std::vector<Item::Id>& items) const
{
  if (tile_x >= 0 && tile_x < width_ && tile_y >= 0 && tile_y < height_)
  {
    return items[(tile_y * width_) + tile_x];
  }
  else
  {
    return Item::invalid;
  }
}

bool LevelBase::collides(const geometry::Rectangle& rect)
{
  for (const auto& aabb : aabbs_)
  {
    if (geometry::isColliding(rect, aabb))
    {
      return true;
    }
  }
  return false;
}
