#ifndef GAME_H_
#define GAME_H_

#include <memory>
#include <vector>

#include "player_input.h"
#include "item.h"
#include "level.h"
#include "player.h"

class Game
{
 public:
  Game()
    : player_(),
      items_(),
      level_()
  {
  }

  bool init();
  void update(unsigned game_tick, const PlayerInput& player_input);

  const Player& get_player() const { return player_; }
  const std::vector<Item>& get_items() const { return items_; }
  const Level& get_level() const { return *level_; }

 private:
  void update_level(unsigned game_tick);
  void update_player(const PlayerInput& player_input);

  bool player_collides(const geometry::Position& position);

  Player player_;
  std::vector<Item> items_;
  std::unique_ptr<Level> level_;
};

#endif  // GAME_H_
