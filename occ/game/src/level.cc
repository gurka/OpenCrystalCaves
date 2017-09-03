#include <utility>

#include "level.h"
#include "math.h"
#include "logger.h"

Level::Level()
  : valid_(false),
    width_(0),
    height_(0),
    tiles_background_(),
    tiles_foreground_(),
    aabbs_(),
    platforms_(),
    objects_(),
    earth_(),
    moon_(),
    volcano_()
{
}

Level::Level(int width,
             int height,
             std::vector<Item::Id> tiles_background,
             std::vector<Item::Id> tiles_foreground,
             std::vector<geometry::Rectangle> aabbs,
             std::vector<geometry::Position> platforms)
  : valid_(true),
    width_(width),
    height_(height),
    tiles_background_(std::move(tiles_background)),
    tiles_foreground_(std::move(tiles_foreground)),
    aabbs_(std::move(aabbs)),
    platforms_(std::move(platforms)),
    objects_(),
    earth_(),
    moon_(),
    volcano_()
{
}

Item::Id Level::get_tile_background(int tile_x, int tile_y) const
{
  return get_tile(tile_x, tile_y, tiles_background_);
}

Item::Id Level::get_tile_foreground(int tile_x, int tile_y) const
{
  return get_tile(tile_x, tile_y, tiles_foreground_);
}

Item::Id Level::get_tile(int tile_x, int tile_y, const std::vector<Item::Id>& items) const
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

void Level::update(unsigned game_tick)
{
  // Clear all objects
  objects_.clear();

  // Update earth
  if (earth_.right)
  {
    earth_.position_x += 1;
    if ((earth_.position_x / 2) + 16 == width_ * 16)
    {
      earth_.right = false;
    }
  }
  else
  {
    earth_.position_x -= 1;
    if (earth_.position_x == 32)
    {
      earth_.right = true;
    }
  }

  // Update moon
  if (moon_.right)
  {
    moon_.position_x += earth_.right ? 2 : 1;
    if (moon_.position_x > earth_.position_x + 72 || (moon_.position_x / 2) + 16 == width_ * 16)
    {
      moon_.right = false;
    }
  }
  else
  {
    moon_.position_x -= !earth_.right ? 2 : 1;
    if (moon_.position_x < earth_.position_x - 64 || moon_.position_x == 32)
    {
      moon_.right = true;
    }
  }

  // Update volcano
  if (volcano_.active && game_tick - volcano_.tick_start >= 81u)
  {
    volcano_.active = false;
    volcano_.tick_start = game_tick;
  }
  else if (!volcano_.active && game_tick - volcano_.tick_start >= 220u)
  {
    volcano_.active = true;
    volcano_.tick_start = game_tick;
  }

  // Add objects
  if (moon_.right)
  {
    // Moon is behind earth, render moon first
    objects_.emplace_back(geometry::Position(moon_.position_x / 2, 0), geometry::Size(16, 16), 634);
    objects_.emplace_back(geometry::Position(earth_.position_x / 2, 0), geometry::Size(16, 16), 632);
  }
  else
  {
    // Moon is in front of earth, render earth first
    objects_.emplace_back(geometry::Position(earth_.position_x / 2, 0), geometry::Size(16, 16), 632);
    objects_.emplace_back(geometry::Position(moon_.position_x / 2, 0), geometry::Size(16, 16), 633);
  }
  if (volcano_.active)
  {
    const auto volcano_sprite_index = ((game_tick - volcano_.tick_start) / 3) % 4;
    objects_.emplace_back(geometry::Position(29 * 16, 2 * 16),
                          geometry::Size(16, 16),
                          752 + volcano_sprite_index);
    objects_.emplace_back(geometry::Position(30 * 16, 2 * 16),
                          geometry::Size(16, 16),
                          748 + volcano_sprite_index);
  }
}
