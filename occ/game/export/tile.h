#ifndef TILE_H_
#define TILE_H_

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

#endif  // TILE_H_
