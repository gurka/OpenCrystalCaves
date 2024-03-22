#pragma once

#include <memory>
#include <vector>

#include "background.h"
#include "enemy.h"
#include "item.h"
#include "level_id.h"
#include "object.h"
#include "player.h"
#include "player_input.h"
#include "tile.h"

class Game
{
 public:
  static std::unique_ptr<Game> create();

  virtual ~Game() = default;

  virtual bool init() = 0;
  virtual void update(unsigned game_tick, const PlayerInput& player_input) = 0;

  virtual const Player& get_player() const = 0;

  virtual LevelId get_level_id() const = 0;

  virtual int get_tile_width() const = 0;
  virtual int get_tile_height() const = 0;

  virtual const Background& get_background() const = 0;
  virtual const Tile& get_tile(int tile_x, int tile_y) const = 0;
  virtual const Item& get_item(int tile_x, int tile_y) const = 0;

  virtual const std::vector<Enemy>& get_enemies() const = 0;

  virtual const std::vector<Object>& get_objects() const = 0;

  virtual unsigned get_score() const = 0;
  virtual unsigned get_num_ammo() const = 0;
  virtual unsigned get_num_lives() const = 0;
  virtual bool has_key() const = 0;

  virtual std::wstring get_debug_info() const = 0;
};
