#include <utility>

#include "level.h"
#include "math.h"

Level::Level()
  : valid_(false),
    width_(0),
    height_(0),
    tiles_background_(),
    tiles_foreground_(),
    aabbs_(),
    platforms_(),
    objects_()
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
    objects_()
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
    return items[pos2index(tile_x, tile_y)];
  }
  else
  {
    return Item::invalid;
  }
}

void Level::update()
{
  // Clear all objects
  objects_.clear();

  // Update earth
  if (earth_.right)
  {
    earth_.position += geometry::Position(1, 0);
    if (earth_.position.x() + 16 == width_ * 16)
    {
      earth_.right = false;
    }
  }
  else
  {
    earth_.position -= geometry::Position(1, 0);
    if (earth_.position.x() == 0)
    {
      earth_.right = true;
    }
  }

  objects_.push_back({earth_.position, geometry::Size(16, 16), 632});

  // Update moon
  if (moon_.right)
  {
    moon_.position += geometry::Position(2, 0);
    if (moon_.position.x() >= earth_.position.x() + 16)
    {
      moon_.right = false;
    }
  }
  else
  {
    moon_.position -= geometry::Position(1, 0);
    if (moon_.position.x() <= earth_.position.x() - 16)
    {
      moon_.right = true;
    }
  }

  const auto moon_earth_distance = math::abs(moon_.position.x() - earth_.position.x());

  if (moon_earth_distance > 8)
  {
    objects_.push_back({moon_.position, geometry::Size(16, 16), 634});
  }
  else if (moon_.right)
  {
    objects_.push_back({moon_.position, geometry::Size(16, 16), 633});
  }
}
