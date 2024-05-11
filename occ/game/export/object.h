#include "geometry.h"

struct Object
{
  Object(geometry::Position position, int sprite_id, int num_sprites, const bool reverse)
    : position(position),
      sprite_id(sprite_id),
      num_sprites(num_sprites),
      reverse(reverse)
  {
  }

  int get_sprite(const int ticks) const
  {
    const auto d = ticks % num_sprites;
    return (reverse ? num_sprites - 1 - d : d) + sprite_id;
  }

  geometry::Position position;
  int sprite_id;
  int num_sprites;
  bool reverse;
};
