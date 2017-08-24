#ifndef GAME_H_
#define GAME_H_

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
      level_(),
      player_collide_x_(false),
      player_collide_y_(false)
  {
  }

  bool init();
  void update(const PlayerInput& player_input);

  const Player& get_player() const { return player_; }
  const std::vector<Item>& get_items() const { return items_; }
  const Level& get_level() const { return level_; }

  // Debug
  bool player_collide_x() const { return player_collide_x_; }
  bool player_collide_y() const { return player_collide_y_; }

 private:
  Player player_;
  std::vector<Item> items_;
  Level level_;

  // Debug
  bool player_collide_x_;
  bool player_collide_y_;
};

#endif  // GAME_H_
