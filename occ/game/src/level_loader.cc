#include "level_loader.h"

#include <cstdio>
#include <fstream>
#include <unordered_map>
#include <utility>

#include "game.h"
#include "level.h"
#include "logger.h"

namespace LevelLoader
{

// https://moddingwiki.shikadi.net/wiki/Crystal_Caves_Map_Format

constexpr int levelLoc = 0x8CE0;
constexpr int levelRows[] = {
  // intro
  5,
  // finale
  6,
  // main
  25,
  24,
  24,
  24,
  24,
  24,
  24,
  23,
  23,
  24,
  24,
  24,
  24,
  24,
  23,
  24,
  24,
};
const std::pair<Sprite, geometry::Size> levelBGs[] = {
  // intro
  {Sprite::SPRITE_STARS_1, {6, 1}},
  // finale
  {Sprite::SPRITE_STARS_1, {6, 1}},
  // main
  {Sprite::SPRITE_ROCKS_1, {2, 2}},
  {Sprite::SPRITE_HEX_ROCKS_1, {4, 2}},
  {Sprite::SPRITE_VERTICAL_WALL_1, {2, 2}},
  {Sprite::SPRITE_COBBLE_1, {2, 2}},
  {Sprite::SPRITE_RED_PANEL_1, {2, 2}},
  {Sprite::SPRITE_PEBBLE_WALL_1, {2, 2}},
  {Sprite::SPRITE_DARK_STONE_1, {3, 2}},
  {Sprite::SPRITE_DIAMOND_WALL_1, {2, 2}},
  {Sprite::SPRITE_COLUMN_AND_KNOB_1, {2, 2}},
  {Sprite::SPRITE_GREEN_SCAFFOLD_1, {3, 2}},
  {Sprite::SPRITE_WOOD_WALL_1, {4, 2}},
  {Sprite::SPRITE_GREY_STONE_1, {2, 2}},
  {Sprite::SPRITE_RED_RECT_1, {4, 2}},
  {Sprite::SPRITE_BRICK_1, {2, 2}},
  {Sprite::SPRITE_RED_SCAFFOLD_1, {4, 2}},
  {Sprite::SPRITE_METAL_BARS_1, {2, 2}},
  {Sprite::SPRITE_BLUE_DIAMOND_1, {2, 2}},
};
// Different levels use different block colours
const Sprite blockColors[] = {
  // Intro 1-2
  Sprite::SPRITE_BLOCK_PEBBLE_NW,
  Sprite::SPRITE_BLOCK_PEBBLE_NW,
  // Main
  Sprite::SPRITE_BLOCK_PEBBLE_NW,
  // 1-8
  Sprite::SPRITE_BLOCK_BROWN_NW,
  Sprite::SPRITE_BLOCK_CYAN_NW,
  Sprite::SPRITE_BLOCK_PEBBLE_NW,
  Sprite::SPRITE_BLOCK_CYAN_NW,
  Sprite::SPRITE_BLOCK_CYAN_NW,
  Sprite::SPRITE_BLOCK_PEBBLE_NW,
  Sprite::SPRITE_BLOCK_PEBBLE_NW,
  Sprite::SPRITE_BLOCK_BLUE_NW,
  // 9-16
  Sprite::SPRITE_BLOCK_PEBBLE_NW,
  Sprite::SPRITE_BLOCK_PEBBLE_NW,
  Sprite::SPRITE_BLOCK_BLUE_NW,
  Sprite::SPRITE_BLOCK_BLUE_NW,
  Sprite::SPRITE_BLOCK_GREEN_NW,
  Sprite::SPRITE_BLOCK_PEBBLE_NW,
  Sprite::SPRITE_BLOCK_METAL_NW,
};

std::unique_ptr<Level> load(const ExeData& exe_data, const LevelId level_id)
{
  LOG_INFO("Loading level %d", static_cast<int>(level_id));
  // Find the location in exe data of the level
  const char* ptr = exe_data.data.c_str() + levelLoc;
  int level;
  for (level = static_cast<int>(LevelId::INTRO); level <= static_cast<int>(LevelId::LEVEL_16); level++)
  {
    if (level == static_cast<int>(level_id))
    {
      break;
    }
    // Skip this level's rows
    for (int row = 0; row < levelRows[level]; row++)
    {
      const size_t len = *ptr;
      ptr++;
      ptr += len;
    }
  }

  // Read the tile ids of the level
  std::vector<int> tile_ids;
  std::vector<int> item_ids;
  int width = 0;
  for (int row = 0; row < levelRows[level]; row++)
  {
    const int len = *ptr;
    if (width == 0)
    {
      width = len;
    }
    ptr++;
    const auto row_str = std::string(ptr).substr(0, len);
    LOG_DEBUG("%s", row_str.c_str());
    for (int i = 0; i < len; i++, ptr++)
    {
      tile_ids.push_back(static_cast<int>(*ptr));
      // TODO
      item_ids.push_back(-1);
    }
  }
  const auto background = levelBGs[static_cast<int>(level_id)];
  return std::make_unique<Level>(level_id,
                                 width,
                                 levelRows[static_cast<int>(level_id)],
                                 background.first,
                                 background.second,
                                 blockColors[static_cast<int>(level_id)],
                                 tile_ids,
                                 std::move(item_ids));
}

}
