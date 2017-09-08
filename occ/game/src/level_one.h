#ifndef LEVEL_ONE_H_
#define LEVEL_ONE_H_

#include "level_base.h"

class LevelOne : public LevelBase
{
 public:
  LevelOne(int width,
           int height,
           std::vector<Item::Id> tiles_background,
           std::vector<Item::Id> tiles_foreground,
           std::vector<geometry::Rectangle> aabbs,
           std::vector<geometry::Position> platforms);
};

#endif  // LEVEL_ONE_H_
