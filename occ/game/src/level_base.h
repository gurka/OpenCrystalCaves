#ifndef LEVEL_BASE_H_
#define LEVEL_BASE_H_

#include <vector>

#include "level.h"
#include "item.h"
#include "geometry.h"
#include "player.h"

class LevelBase : public Level
{
 public:
  LevelBase(int width,
            int height,
            Player player,
            std::vector<Item::Id> tiles_background,
            std::vector<Item::Id> tiles_foreground,
            std::vector<geometry::Rectangle> aabbs,
            std::vector<geometry::Position> platforms,
            std::vector<MovingPlatform> moving_platforms);

  virtual ~LevelBase() = default;

  // Can be overriden by specific levels, but this method
  // should be still be called even if it's overriden
  virtual void update(unsigned game_tick);

  int get_tile_width() const override { return width_; }
  int get_tile_height() const override { return height_; }

  Player& get_player() override { return player_; }
  const Player& get_player() const override { return player_; }

  Item::Id get_tile_background(int tile_x, int tile_y) const override;
  Item::Id get_tile_foreground(int tile_x, int tile_y) const override;

  const std::vector<MovingPlatform>& get_moving_platforms() const override { return moving_platforms_; }

  const std::vector<geometry::Rectangle>& get_aabbs() const override { return aabbs_; }
  const std::vector<geometry::Position>& get_platforms() const override { return platforms_; }

  const std::vector<Object>& get_objects() const override { return objects_; }

  bool collides(const geometry::Rectangle& rect) override;

 protected:
  Item::Id get_tile(int tile_x, int tile_y, const std::vector<Item::Id>& items) const;

  // Size of level in number of tiles
  int width_;
  int height_;

  // The player
  Player player_;

  // Static tiles, in order
  std::vector<Item::Id> tiles_background_;
  std::vector<Item::Id> tiles_foreground_;

  // Dynamic tiles
  std::vector<MovingPlatform> moving_platforms_;

  // For collision detection
  std::vector<geometry::Rectangle> aabbs_;
  std::vector<geometry::Position> platforms_;  // FIXME: Rectangle?

  // Additional objects that should be rendered
  std::vector<Object> objects_;
};

#endif  // LEVEL_BASE_H_
