#pragma once

#include <vector>

#include "entrance.h"
#include "geometry.h"
#include "item.h"
#include "level_id.h"
#include "moving_platform.h"
#include "sprite.h"
#include "tile.h"

struct Level
{
  Level(LevelId level_id,
        int width,
        int height,
        geometry::Position player_spawn,
        std::vector<MovingPlatform>&& moving_platforms,
        std::vector<Entrance>&& entrances,
        std::vector<int>&& bgs,
        std::vector<Tile>&& tiles,
        std::vector<Item>&& items,
        bool has_earth,
        bool has_moon);

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

  std::vector<MovingPlatform> moving_platforms;
  std::vector<Entrance> entrances;
  bool has_earth = false;
  bool has_moon = false;
};
