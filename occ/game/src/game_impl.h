#ifndef GAME_IMPL_H_
#define GAME_IMPL_H_

#include "game.h"

#include <memory>
#include <vector>

#include "player_input.h"
#include "object_manager.h"
#include "level.h"
#include "player.h"

class GameImpl : public Game
{
 public:
  GameImpl()
    : player_(),
      object_manager_(),
      level_(),
      objects_()
  {
  }

  bool init() override;
  void update(unsigned game_tick, const PlayerInput& player_input) override;

  const Player& get_player() const override { return player_; }

  LevelId get_level_id() const override { return level_->level_id; }

  int get_tile_width() const override { return level_->width; }
  int get_tile_height() const override { return level_->height; }

  const Background& get_background() const override;
  const Tile& get_tile(int tile_x, int tile_y) const override;
  const Item& get_item(int tile_x, int tile_y) const override;

  const std::vector<Object>& get_objects() const override { return objects_; }

 private:
  void update_level();
  void update_player(const PlayerInput& player_input);
  void update_items();

  bool player_collides(const geometry::Position& player_position);
  bool player_on_platform(const geometry::Position& player_position);

  void remove_item(int tile_x, int tile_y);

  Player player_;
  ObjectManager object_manager_;
  std::unique_ptr<Level> level_;
  std::vector<Object> objects_;
};

#endif  // GAME_IMPL_H_
