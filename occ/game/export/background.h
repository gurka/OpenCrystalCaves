#ifndef BACKGROUND_H_
#define BACKGROUND_H_

#include "geometry.h"

struct Background
{
  Background(int sprite_id, geometry::Size size_in_tiles)
    : sprite_id(sprite_id),
      size_in_tiles(size_in_tiles)
  {
  }

  int sprite_id;
  geometry::Size size_in_tiles;
};


#endif  // BACKGROUND_H_
