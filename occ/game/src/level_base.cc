#include "level_base.h"

#include <utility>

#include "geometry.h"

LevelBase::LevelBase(int width,
                     int height,
                     Player player,
                     std::vector<Item::Id> tiles_background,
                     std::vector<Item::Id> tiles_foreground,
                     std::vector<geometry::Rectangle> aabbs,
                     std::vector<geometry::Position> platforms,
                     std::vector<MovingPlatform> moving_platforms)
  : width_(width),
    height_(height),
    player_(std::move(player)),
    tiles_background_(std::move(tiles_background)),
    tiles_foreground_(std::move(tiles_foreground)),
    moving_platforms_(std::move(moving_platforms)),
    aabbs_(std::move(aabbs)),
    platforms_(std::move(platforms)),
    objects_()
{
}

void LevelBase::update(unsigned game_tick)
{
  // game_tick is not used here, but can be used for Levels that override update
  (void)game_tick;

  // Update all MovingPlatforms
  for (auto& platform : moving_platforms_)
  {
    // Update platform
    const auto platform_velocity = platform.forward ? platform.velocity_forward : -platform.velocity_forward;
    const auto player_on_platform = (player_.position.y() + player_.size.y() == platform.position.y()) &&
                                    (player_.position.x() < platform.position.x() + 16) &&
                                    (player_.position.x() + player_.size.x() > platform.position.x());

    // Move player if standing on platform
    if (player_on_platform)
    {
      // Only move player if not colliding with any static objects
      const auto new_player_pos = player_.position + platform_velocity;
      if (!collides(geometry::Rectangle(new_player_pos, player_.size)))
      {
        player_.position = new_player_pos;
      }
    }

    // Move platform
    platform.position += platform_velocity;

    // Check if platform reached end / start
    if ((platform.forward && platform.position == platform.position_end) ||
        (!platform.forward && platform.position == platform.position_start))
    {
      // Change direction
      platform.forward = !platform.forward;
    }
  }
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
