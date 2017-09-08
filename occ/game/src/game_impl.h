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
      level_()
  {
  }

  bool init() override;
  void update(unsigned game_tick, const PlayerInput& player_input) override;

  const Player& get_player() const override { return player_; }
  const std::vector<Item>& get_items() const override { return items_; }
  const Level& get_level() const override { return *level_; }

 private:
  void update_level(unsigned game_tick);
  void update_player(const PlayerInput& player_input);

  bool player_collides(const geometry::Position& position);

  Player player_;
  std::vector<Item> items_;
  std::unique_ptr<Level> level_;
};

#endif  // GAME_IMPL_H_
