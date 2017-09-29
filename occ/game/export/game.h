#ifndef GAME_H_
#define GAME_H_

#include <memory>
#include <vector>

#include "player_input.h"
#include "player.h"
#include "level_id.h"
#include "object.h"

struct Background
{
  Background()
    : valid(false),
      sprite_id(-1),
      size_in_tiles(0, 0)
  {
  }

  Background(int sprite_id, geometry::Size size_in_tiles)
    : valid(true),
      sprite_id(sprite_id),
      size_in_tiles(size_in_tiles)
  {
  }

  bool valid;
  int sprite_id;
  geometry::Size size_in_tiles;

  static const Background INVALID;
};

class Tile
{
 public:
  Tile()
    : valid_(false),
      sprite_(-1),
      sprite_count_(0),
      flags_(0)
  {
  }

  Tile(int sprite, int sprite_count, int flags)
    : valid_(true),
      sprite_(sprite),
      sprite_count_(sprite_count),
      flags_(flags)
  {
  }

  bool valid() const { return valid_; }

  int get_sprite() const { return sprite_; }
  int get_sprite_count() const { return sprite_count_; }

  bool is_solid()           const { return (flags_ & 0x01) != 0; }
  bool is_solid_top()       const { return (flags_ & 0x02) != 0; }
  bool is_damage()          const { return (flags_ & 0x04) != 0; }
  bool is_death()           const { return (flags_ & 0x08)  != 0; }
  bool is_animated()        const { return (flags_ & 0x10) != 0; }
  bool is_render_in_front() const { return (flags_ & 0x20) != 0; }

  static const Tile INVALID;

 private:
  bool valid_;
  int sprite_;
  int sprite_count_;
  int flags_;
};

class Item
{
 public:
  Item()
    : valid_(false),
      sprite_(-1),
      type_(0),
      amount_(0)
  {
  }

  Item(int sprite, int type, int amount)
    : valid_(true),
      sprite_(sprite),
      type_(type),
      amount_(amount)
  {
  }

  bool valid() const { return valid_; }

  int get_sprite() const { return sprite_; }
  int get_type() const { return type_; }
  int get_amount() const { return amount_; }

  static const Item INVALID;

 private:
  bool valid_;
  int sprite_;
  int type_;
  int amount_;
};

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

  virtual const std::vector<Object>& get_objects() const = 0;
};

#endif  // GAME_H_
