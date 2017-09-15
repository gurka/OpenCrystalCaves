#ifndef LEVEL_IMPL_H_
#define LEVEL_IMPL_H_

#include <vector>

#include "level.h"
#include "item.h"
#include "geometry.h"
#include "object.h"
#include "moving_platform.h"

class LevelImpl : public Level
{
 public:
  LevelImpl(LevelId level_id,
            int width,
            int height,
            const geometry::Position& player_spawn,
            Background background,
            std::vector<Item::Id> tiles_foreground,
            std::vector<Item::Id> tiles_score,
            std::vector<MovingPlatform> moving_platforms);

  // From Level
  LevelId get_level_id() const override { return level_id_; }
  int get_tile_width() const override { return width_; }
  int get_tile_height() const override { return height_; }
  const Background& get_background() const override { return background_; }
  Item::Id get_tile_foreground(int tile_x, int tile_y) const override;
  Item::Id get_tile_score(int tile_x, int tile_y) const override;
  const std::vector<Object>& get_objects() const override { return objects_; }

  const geometry::Position& get_player_spawn() const { return player_spawn_; }
  void update(unsigned game_tick);

  // Used by Game to update the moving platforms
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

  // Objects
  std::vector<Object> objects_;
};

#endif  // LEVEL_IMPL_H_
