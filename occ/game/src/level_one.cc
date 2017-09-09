#include "level_one.h"

LevelOne::LevelOne(int width,
                   int height,
                   std::vector<Item::Id> tiles_background,
                   std::vector<Item::Id> tiles_foreground,
                   std::vector<Item::Id> tiles_score)
  : LevelBase(width,
              height,
              std::move(tiles_background),
              std::move(tiles_foreground),
              std::move(tiles_score),
              {
                // MovingPlatforms
                {
                  geometry::Position(36 * 16,  7 * 16),
                  geometry::Position(36 * 16, 22 * 16),
                  2,
                  616,
                  4
                },
              })
{
}

geometry::Position LevelOne::get_player_spawn() const
{
  return geometry::Position(4 * 16, 22 * 16);
}

void LevelOne::update_level(unsigned game_tick)
{
  // Nothing to be done here!
  (void)game_tick;
}
