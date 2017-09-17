#ifndef LEVEL_H_
#define LEVEL_H_

#include <vector>

#include "game.h"
#include "item.h"
#include "geometry.h"
#include "moving_platform.h"

class Level
{
 public:
  Level(LevelId level_id,
        int width,
        int height,
        const geometry::Position& player_spawn,
        Background background,
        std::vector<Item::Id> tiles_foreground,
        std::vector<Item::Id> tiles_score,
        std::vector<MovingPlatform> moving_platforms);

  LevelId get_level_id() const { return level_id_; }

  int get_tile_width() const { return width_; }
  int get_tile_height() const { return height_; }

  const Background& get_background() const { return background_; }
  Item::Id get_tile_foreground(int tile_x, int tile_y) const;
  Item::Id get_tile_score(int tile_x, int tile_y) const;
  const std::vector<MovingPlatform>& get_moving_platforms() const { return moving_platforms_; }

  // Used by Game
  const geometry::Position& get_player_spawn() const { return player_spawn_; }
  std::vector<MovingPlatform>& get_moving_platforms() { return moving_platforms_; }

 protected:
  Item::Id get_tile(int tile_x, int tile_y, const std::vector<Item::Id>& items) const;

  LevelId level_id_;

  // Size of level in number of tiles
  int width_;
  int height_;

  geometry::Position player_spawn_;

  // Static tiles, in order
  Background background_;
  std::vector<Item::Id> tiles_foreground_;
  std::vector<Item::Id> tiles_score_;

  // Moving platforms
  std::vector<MovingPlatform> moving_platforms_;
};

#endif  // LEVEL_H_
