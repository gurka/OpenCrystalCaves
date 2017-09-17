#ifndef GAME_H_
#define GAME_H_

#include <memory>
#include <vector>

#include "player_input.h"
#include "player.h"
#include "item.h"
#include "level_id.h"
#include "background.h"
#include "object.h"

class Game
{
 public:
  static std::unique_ptr<Game> create();

  virtual ~Game() = default;

  virtual bool init() = 0;
  virtual void update(unsigned game_tick, const PlayerInput& player_input) = 0;

  virtual const Player& get_player() const = 0;

  virtual const std::vector<Item>& get_items() const = 0;

  // Level functions
  virtual LevelId get_level_id() const = 0;
  virtual int get_tile_width() const = 0;
  virtual int get_tile_height() const = 0;
  virtual const Background& get_background() const = 0;
  virtual Item::Id get_tile(int tile_x, int tile_y) const = 0;
  virtual const std::vector<Object>& get_objects() const = 0;
};

#endif  // GAME_H_
