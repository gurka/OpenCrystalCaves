#include "level_base.h"

#include <utility>

#include "geometry.h"

LevelBase::LevelBase(int width,
                     int height,
                     std::vector<Item::Id> tiles_background,
                     std::vector<Item::Id> tiles_foreground,
                     std::vector<Item::Id> tiles_score,
                     std::vector<MovingPlatform> moving_platforms)
  : width_(width),
    height_(height),
    tiles_background_(std::move(tiles_background)),
    tiles_foreground_(std::move(tiles_foreground)),
    tiles_score_(std::move(tiles_score)),
    moving_platforms_(std::move(moving_platforms)),
    objects_()
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

Item::Id LevelBase::get_tile_score(int tile_x, int tile_y) const
{
  return get_tile(tile_x, tile_y, tiles_score_);
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

void LevelBase::update(unsigned game_tick)
{
  // Clear objects
  objects_.clear();

  // Add MovingPlatform Objects
  for (const auto& platform : moving_platforms_)
  {
    objects_.emplace_back(platform.position, geometry::Size(16, 16), platform.sprite_id, platform.num_sprites);
  }

  // Call the specific Level's update
  update_level(game_tick);
}
