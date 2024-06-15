#pragma once

#include <vector>

#include "entrance.h"
#include "geometry.h"
#include "level_id.h"
#include "moving_platform.h"
#include "sprite.h"
#include "tile.h"

struct Level
{
  Level(LevelId level_id,
        int width,
        int height,
        const Sprite bg_sprite,
        geometry::Size bg_tile_size,
        const std::vector<int>& tile_ids,
        std::vector<int>&& item_ids);

  LevelId level_id;

  int width;
  int height;

  geometry::Position player_spawn;

  const Tile& get_tile(const int x, const int y) const;
  int get_bg(const int x, const int y) const;

  std::vector<int> bgs;
  std::vector<Tile> tiles;
  std::vector<int> item_ids;

  std::vector<MovingPlatform> moving_platforms;
  std::vector<Entrance> entrances;
  bool has_earth = false;
  bool has_moon = false;
};
