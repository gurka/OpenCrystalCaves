#ifndef LEVEL_H_
#define LEVEL_H_

#include <vector>

#include "level_id.h"
#include "geometry.h"
#include "moving_platform.h"

struct Level
{
  Level(LevelId level_id,
        int width,
        int height,
        const geometry::Position& player_spawn,
        int background_id,
        std::vector<int>&& tile_ids,
        std::vector<int>&& item_ids,
        std::vector<MovingPlatform>&& moving_platforms)
    : level_id(level_id),
      width(width),
      height(height),
      player_spawn(player_spawn),
      background_id(background_id),
      tile_ids(std::move(tile_ids)),
      item_ids(std::move(item_ids)),
      moving_platforms(std::move(moving_platforms))
  {
  }

  LevelId level_id;

  int width;
  int height;

  geometry::Position player_spawn;

  int background_id;
  std::vector<int> tile_ids;
  std::vector<int> item_ids;

  std::vector<MovingPlatform> moving_platforms;
};

#endif  // LEVEL_H_
