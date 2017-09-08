#include "level_one.h"

LevelOne::LevelOne(int width,
                   int height,
                   std::vector<Item::Id> tiles_background,
                   std::vector<Item::Id> tiles_foreground,
                   std::vector<geometry::Position> platforms)
  : LevelBase(width,
              height,
              std::move(tiles_background),
              std::move(tiles_foreground),
              std::move(platforms),
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

void LevelOne::update(unsigned game_tick)
{
  (void)game_tick;
}
