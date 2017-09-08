#ifndef GAME_H_
#define GAME_H_

#include <memory>
#include <vector>

class PlayerInput;
class Player;
class Item;
class Level;

class Game
{
 public:
  static std::unique_ptr<Game> create();

  virtual ~Game() = default;

  virtual bool init() = 0;
  virtual void update(unsigned game_tick, const PlayerInput& player_input) = 0;

  virtual const Player& get_player() const = 0;
  virtual const std::vector<Item>& get_items() const = 0;
  virtual const Level& get_level() const = 0;
};

#endif  // GAME_H_
