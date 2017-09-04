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
            std::vector<Item::Id> tiles_background,
            std::vector<Item::Id> tiles_foreground,
            std::vector<geometry::Rectangle> aabbs,
            std::vector<geometry::Position> platforms);

  virtual ~LevelBase() = default;

  // Should be overriden by each level
  virtual void update(unsigned game_tick) = 0;

  int get_tile_width() const override { return width_; }
  int get_tile_height() const override { return height_; }

  Player& get_player() override { return player_; }
  const Player& get_player() const override { return player_; }

  Item::Id get_tile_background(int tile_x, int tile_y) const override;
  Item::Id get_tile_foreground(int tile_x, int tile_y) const override;

  const std::vector<geometry::Rectangle>& get_aabbs() const override { return aabbs_; }
  const std::vector<geometry::Position>& get_platforms() const override { return platforms_; }

  // Dynamic things in the level
  const std::vector<Object>& get_objects() const override { return objects_; }
  const std::vector<geometry::Position>& get_moving_platforms() const override { return moving_platforms_; }

  // Helpers
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

  // Bounding boxes for collision
  std::vector<geometry::Rectangle> aabbs_;
  std::vector<geometry::Position> platforms_;  // FIXME: Rectangle?

  // Dynamic objects
  std::vector<Object> objects_;  // Used for rendering
  std::vector<geometry::Position> moving_platforms_;  // Used for collision
};

#endif  // LEVEL_BASE_H_
