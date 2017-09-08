#ifndef LEVEL_MAINLEVEL_H_
#define LEVEL_MAINLEVEL_H_

#include "level_base.h"

class LevelMainLevel : public LevelBase
{
 public:
  LevelMainLevel(int width,
                 int height,
                 std::vector<Item::Id> tiles_background,
                 std::vector<Item::Id> tiles_foreground);

  // From LevelBase
  geometry::Position get_player_spawn() const override;
  void update_level(unsigned game_tick) override;

 private:
  struct
  {
    bool right = false;  // true -> going right, false -> going left
    int position_x = 128;  // subpixel: earth.position.x() = position_x / 2;
  } earth_;

  struct
  {
    bool right = false;  // true -> going right, false -> going left
    int position_x = 160;  // subpixel: moon.position.x() = position_x / 2;
  } moon_;

  struct
  {
    // 4 different sprites (times 2 for left/right)
    // Changes sprite 27 times (4 + 4 + 4 + 4 + 4 + 4 + 3)
    // Each sprite visible for 3 ticks
    // Visible for 27 * 3 = 81 ticks
    // Visible for 4630ms = 81 ticks
    // Idle for 12567ms = 220 ticks

    // Sprite ids:
    // Left  Right
    //  752   748
    //  753   749
    //  754   750
    //  755   751
    bool active = false;
    unsigned tick_start = 0u;  // tick when active changed
  } volcano_;
};

#endif  // LEVEL_MAINLEVEL_H_
