#include "level.h"

Level::Level(LevelId level_id,
             int width,
             int height,
             geometry::Position player_spawn,
             std::vector<MovingPlatform>&& moving_platforms,
             std::vector<Entrance>&& entrances,
             std::vector<int>&& bgs,
             std::vector<Tile>&& tiles,
             std::vector<Item>&& items,
             bool has_earth,
             bool has_moon)
  : level_id(level_id),
    width(width),
    height(height),
    player_spawn(player_spawn),
    moving_platforms(std::move(moving_platforms)),
    entrances(std::move(entrances)),
    bgs(std::move(bgs)),
    tiles(std::move(tiles)),
    items(std::move(items)),
    has_earth(has_earth),
    has_moon(has_moon)
{
}

const Tile& Level::get_tile(const int x, const int y) const
{
  if (x < 0 || y >= width || y < 0 || y >= height)
  {
    return Tile::INVALID;
  }
  return tiles[(y * width) + x];
}

int Level::get_bg(const int x, const int y) const
{
  if (x < 0 || y >= width || y < 0 || y >= height)
  {
    return -1;
  }
  return bgs[(y * width) + x];
}

const Item& Level::get_item(const int x, const int y) const
{
  if (x < 0 || y >= width || y < 0 || y >= height)
  {
    return Item::INVALID;
  }
  return items[(y * width) + x];
}

void Level::remove_item(const int x, const int y)
{
  items[(y * width) + x].invalidate();
}
