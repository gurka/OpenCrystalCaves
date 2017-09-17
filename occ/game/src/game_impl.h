#ifndef GAME_IMPL_H_
#define GAME_IMPL_H_

#include "game.h"

#include <memory>
#include <vector>

#include "player_input.h"
#include "item.h"
#include "level.h"
#include "player.h"

class GameImpl : public Game
{
 public:
  GameImpl()
    : player_(),
      items_(),
      level_(),
      objects_()
  {
  }

  bool init() override;
  void update(unsigned game_tick, const PlayerInput& player_input) override;

  const Player& get_player() const override { return player_; }

  const std::vector<Item>& get_items() const override { return items_; }

  LevelId get_level_id() const { return level_->get_level_id(); }
  int get_tile_width() const { return level_->get_tile_width(); }
  int get_tile_height() const { return level_->get_tile_height(); }
  const Background& get_background() const { return level_->get_background(); }
  Item::Id get_tile_foreground(int tile_x, int tile_y) const { return level_->get_tile_foreground(tile_x, tile_y); }
  Item::Id get_tile_score(int tile_x, int tile_y) const { return level_->get_tile_score(tile_x, tile_y); }
  const std::vector<Object>& get_objects() const { return objects_; }

 private:
  void update_level();
  void update_player(const PlayerInput& player_input);

  bool player_collides(const geometry::Position& position);
  bool player_on_platform(const geometry::Position& position);

  Player player_;
  std::vector<Item> items_;
  std::unique_ptr<Level> level_;
  std::vector<Object> objects_;
};

#endif  // GAME_IMPL_H_
