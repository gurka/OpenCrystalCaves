#include "level_loader.h"

#include <cstdio>
#include <fstream>
#include <nlohmann/json.hpp>
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
const char* levelBGs[] = {
  // intro
  "stars",
  // finale
  "stars",
  // main
  "rocks",
  "",
  "",
  "",
  "red panel",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
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
                                 geometry::Position(4 * 16, 22 * 16),  // Player spawn
                                 background,
                                 tile_ids,
                                 std::move(item_ids));
}

}
