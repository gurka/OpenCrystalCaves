#pragma once

enum TileFlags
{
  TILE_SOLID = 0x01,
  TILE_SOLID_TOP = 0x02,
  TILE_DAMAGE = 0x04,
  TILE_DEATH = 0x08,
  TILE_ANIMATED = 0x10,
  TILE_RENDER_IN_FRONT = 0x20,
  TILE_BLOCKS_SLIME = 0x40,
};

class Tile
{
 public:
  Tile() : valid_(false), sprite_(-1), sprite_count_(0), flags_(0) {}

  Tile(int sprite, int sprite_count, int flags) : valid_(true), sprite_(sprite), sprite_count_(sprite_count), flags_(flags) {}

  bool valid() const { return valid_; }

  int get_sprite() const { return sprite_; }
  int get_sprite_count() const { return sprite_count_; }

  bool is_solid() const { return !!(flags_ & TILE_SOLID); }
  bool is_solid_top() const { return (flags_ & 0x02) != 0; }
  bool is_damage() const { return (flags_ & 0x04) != 0; }
  bool is_death() const { return (flags_ & 0x08) != 0; }
  bool is_animated() const { return (flags_ & 0x10) != 0; }
  bool is_render_in_front() const { return (flags_ & 0x20) != 0; }
  bool is_solid_for_slime() const { return !!(flags_ & (TILE_BLOCKS_SLIME | TILE_SOLID)); }

  static const Tile INVALID;

 private:
  bool valid_;
  int sprite_;
  int sprite_count_;
  int flags_;
};
