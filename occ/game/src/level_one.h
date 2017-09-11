#ifndef LEVEL_ONE_H_
#define LEVEL_ONE_H_

#include "level_base.h"

class LevelOne : public LevelBase
{
 public:
  LevelOne(int width,
           int height,
           std::vector<Item::Id> tiles_foreground,
           std::vector<Item::Id> tiles_score);

  // From LevelBase
  geometry::Position get_player_spawn() const override;
  void update_level(unsigned game_tick) override;
};

#endif  // LEVEL_ONE_H_
