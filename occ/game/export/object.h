#ifndef OBJECT_H_
#define OBJECT_H_

#include "geometry.h"

struct Object
{
  Object(geometry::Position position, int sprite_id, int num_sprites)
    : position(position),
      sprite_id(sprite_id),
      num_sprites(num_sprites)
  {
  }

  geometry::Position position;
  int sprite_id;
  int num_sprites;
};

#endif  // OBJECT_H_
