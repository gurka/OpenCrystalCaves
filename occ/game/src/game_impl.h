#ifndef GAME_IMPL_H_
#define GAME_IMPL_H_

#include "game.h"

#include <memory>
#include <vector>

#include "player_input.h"
#include "object_manager.h"
#include "level.h"
#include "player.h"
#include "missile.h"
#include "explosion.h"
#include "enemy.h"

class GameImpl : public Game
{
 public:
  GameImpl()
    : player_(),
      object_manager_(),
      level_(),
      enemies_(),
      objects_(),
      score_(0u),
      num_ammo_(0u),
      num_lives_(0u),
      missile_(),
      explosion_()
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

  const std::vector<Enemy>& get_enemies() const override { return enemies_; }

  const std::vector<Object>& get_objects() const override { return objects_; }

  unsigned get_score() const override { return score_; }
  unsigned get_num_ammo() const override { return num_ammo_; }
  unsigned get_num_lives() const override { return num_lives_; }

  std::string get_debug_info() const override;

 private:
  void update_level();
  void update_player(const PlayerInput& player_input);
  void update_items();
  void update_missile();

  bool collides_solid(const geometry::Position& position, const geometry::Size& size);
  int collides_enemy(const geometry::Position& position, const geometry::Size& size);
  bool player_on_platform(const geometry::Position& player_position);

  void remove_item(int tile_x, int tile_y);

  Player player_;
  ObjectManager object_manager_;
  std::unique_ptr<Level> level_;
  std::vector<Enemy> enemies_;
  std::vector<Object> objects_;

  unsigned score_;
  unsigned num_ammo_;
  unsigned num_lives_;

  Missile missile_;
  Explosion explosion_;
};

#endif  // GAME_IMPL_H_
