#include "level_loader.h"

#include <cstdio>
#include <fstream>
#include <nlohmann/json.hpp>
#include <unordered_map>
#include <utility>

#include "game.h"
#include "level.h"
#include "logger.h"

namespace
{

const std::unordered_map<LevelId, std::string> level_filename = {
  {LevelId::MAIN_LEVEL, "media/mainlevel.json"},
  {LevelId::LEVEL_1, "media/level1.json"},
};

}

namespace LevelLoader
{

std::unique_ptr<Level> load_level(LevelId level_id, const ObjectManager& object_manager)
{
  // Check if level is valid
  if (level_filename.count(level_id) == 0)
  {
    LOG_CRITICAL("Unknown level id: %d", static_cast<int>(level_id));
    return std::unique_ptr<Level>();
  }

  const auto& filename = level_filename.at(level_id);

  // Open and parse JSON file
  std::ifstream level_file(filename);
  if (!level_file.good())
  {
    LOG_CRITICAL("Could not open file \"%s\"", filename.c_str());
    return std::unique_ptr<Level>();
  }
  nlohmann::json level_json;
  level_file >> level_json;
  level_file.close();

  // Check attributes
  if (level_json.count("Width") == 0 || !level_json["Width"].is_number())
  {
    LOG_CRITICAL("Level is missing \"Width\" attribute or wrong type!");
    return std::unique_ptr<Level>();
  }
  if (level_json.count("Height") == 0 || !level_json["Height"].is_number())
  {
    LOG_CRITICAL("Level is missing \"Height\" attribute or wrong type!");
    return std::unique_ptr<Level>();
  }
  if (level_json.count("Background") == 0 || !level_json["Background"].is_string())
  {
    LOG_CRITICAL("Level is missing \"Background\" attribute or wrong type!");
    return std::unique_ptr<Level>();
  }
  if (level_json.count("TileIds") == 0 || !level_json["TileIds"].is_array())
  {
    LOG_CRITICAL("Level is missing \"TileIds\" attribute or wrong type!");
    return std::unique_ptr<Level>();
  }
  if (level_json.count("ItemIds") == 0 || !level_json["ItemIds"].is_array())
  {
    LOG_CRITICAL("Level is missing \"ItemIds\" attribute or wrong type!");
    return std::unique_ptr<Level>();
  }

  // Read attributes
  const auto width = level_json["Width"].get<int>();
  const auto height = level_json["Height"].get<int>();
  const auto background = level_json["Background"].get<std::string>();
  auto tile_ids = level_json["TileIds"].get<std::vector<int>>();  // TODO: Tile::Id ?
  auto item_ids = level_json["ItemIds"].get<std::vector<int>>();  // TODO: Item::Id ?

  LOG_INFO("Loaded level from '%s'", filename.c_str());
  LOG_DEBUG("Level information: width=%d height=%d", width, height);

  if (level_id == LevelId::MAIN_LEVEL)
  {
    return std::make_unique<Level>(level_id,
                                   width,
                                   height,
                                   geometry::Position(32, 48),  // Player spawn
                                   background,
                                   tile_ids,
                                   std::move(item_ids),
                                   std::vector<MovingPlatform>({{// Vertical
                                                                 geometry::Position(38 * 16, 7 * 16),
                                                                 geometry::Position(38 * 16, 22 * 16),
                                                                 2,
                                                                 616,
                                                                 4},
                                                                {// Horizontal
                                                                 geometry::Position(11 * 16, 8 * 16),
                                                                 geometry::Position(7 * 16, 8 * 16),
                                                                 2,
                                                                 612,
                                                                 4}}),
                                   object_manager,
                                   false);
  }
  else if (level_id == LevelId::LEVEL_1)
  {
    return std::make_unique<Level>(level_id,
                                   width,
                                   height,
                                   geometry::Position(4 * 16, 22 * 16),  // Player spawn
                                   background,
                                   tile_ids,
                                   std::move(item_ids),
                                   std::vector<MovingPlatform>({
                                     {geometry::Position(36 * 16, 7 * 16), geometry::Position(36 * 16, 22 * 16), 2, 616, 4},
                                   }),
                                   object_manager,
                                   false);
  }
  else
  {
    return std::unique_ptr<Level>();
  }
}

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

std::unique_ptr<Level> load(ExeData& exe_data, const LevelId level_id, const ObjectManager& object_manager)
{
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
                                 std::move(item_ids),
                                 // TODO: moving platforms
                                 std::vector<MovingPlatform>({
                                   {geometry::Position(36 * 16, 7 * 16), geometry::Position(36 * 16, 22 * 16), 2, 616, 4},
                                 }),
                                 object_manager,
                                 true);
}

}
