#include "level.h"

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
