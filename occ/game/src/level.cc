#include "level.h"

#include <utility>

#include "geometry.h"

Level::Level(LevelId level_id,
             int width,
             int height,
             const geometry::Position& player_spawn,
             Background background,
             std::vector<Item::Id> tiles,
             std::vector<MovingPlatform> moving_platforms)
  : level_id_(level_id),
    width_(width),
    height_(height),
    player_spawn_(player_spawn),
    background_(std::move(background)),
    tiles_(std::move(tiles)),
    moving_platforms_(std::move(moving_platforms))
{
}

Item::Id Level::get_tile(int tile_x, int tile_y) const
{
  if (tile_x >= 0 && tile_x < width_ && tile_y >= 0 && tile_y < height_)
  {
    return tiles_[(tile_y * width_) + tile_x];
  }
  else
  {
    return Item::invalid;
  }
}
