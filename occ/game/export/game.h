#ifndef GAME_H_
#define GAME_H_

#include <vector>

#include "input.h"
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
  void update(const Input& input);

  const Player& get_player() const { return player_; }
  const std::vector<Item>& get_items() const { return items_; }
  const Level& get_level() const { return level_; }

 private:
  Player player_;
  std::vector<Item> items_;
  Level level_;
};

#endif  // GAME_H_
