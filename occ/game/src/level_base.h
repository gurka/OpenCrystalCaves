#ifndef LEVEL_BASE_H_
#define LEVEL_BASE_H_

#include <vector>

#include "level.h"
#include "item.h"
#include "geometry.h"
#include "object.h"
#include "moving_platform.h"

class LevelBase : public Level
{
 public:
  LevelBase(int width,
            int height,
            std::vector<Item::Id> tiles_background,
            std::vector<Item::Id> tiles_foreground,
            std::vector<geometry::Position> platforms,
            std::vector<MovingPlatform> moving_platforms);

  virtual ~LevelBase() = default;

  // From Level
  int get_tile_width() const override { return width_; }
  int get_tile_height() const override { return height_; }
  Item::Id get_tile_background(int tile_x, int tile_y) const override;
  Item::Id get_tile_foreground(int tile_x, int tile_y) const override;
  const std::vector<Object>& get_objects() const override { return objects_; }

  void update(unsigned game_tick);

  // Must be overriden by Levels
  virtual geometry::Position get_player_spawn() const = 0;
  virtual void update_level(unsigned game_tick) = 0;

  // Used by Game
  std::vector<MovingPlatform>& get_moving_platforms() { return moving_platforms_; }
  std::vector<geometry::Position>& get_platforms() { return platforms_; }

 protected:
  Item::Id get_tile(int tile_x, int tile_y, const std::vector<Item::Id>& items) const;

  // Size of level in number of tiles
  int width_;
  int height_;

  // Static tiles, in order
  std::vector<Item::Id> tiles_background_;
  std::vector<Item::Id> tiles_foreground_;

  std::vector<MovingPlatform> moving_platforms_;
  std::vector<geometry::Position> platforms_;  // FIXME: Rectangle?

  // Objects
  std::vector<Object> objects_;
};

#endif  // LEVEL_BASE_H_
