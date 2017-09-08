#ifndef OBJECT_H_
#define OBJECT_H_

#include "geometry.h"

struct Object
{
  Object(geometry::Position position, geometry::Size size, int sprite_id, int num_sprites)
    : position(position),
      size(size),
      sprite_id(sprite_id),
      num_sprites(num_sprites)
  {
  }

  geometry::Position position;
  geometry::Size size;
  int sprite_id;
  int num_sprites;  // >1 is animated
};

#endif  // OBJECT_H_
