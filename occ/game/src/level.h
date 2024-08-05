#pragma once

#include <vector>

#include "enemy.h"
#include "entrance.h"
#include "geometry.h"
#include "hazard.h"
#include "item.h"
#include "level_id.h"
#include "moving_platform.h"
#include "sprite.h"
#include "tile.h"

struct Level
{
  LevelId level_id;

  int width;
  int height;

  geometry::Position player_spawn;

  const Tile& get_tile(const int x, const int y) const;
  int get_bg(const int x, const int y) const;
  const Item& get_item(const int x, const int y) const;
  void remove_item(const int x, const int y);

  std::vector<int> bgs;
  std::vector<Tile> tiles;
  std::vector<Item> items;

  std::vector<Enemy> enemies;
  std::vector<std::unique_ptr<Hazard>> hazards;
  std::vector<MovingPlatform> moving_platforms;
  std::vector<Entrance> entrances;
  bool has_earth = false;
  bool has_moon = false;
};
