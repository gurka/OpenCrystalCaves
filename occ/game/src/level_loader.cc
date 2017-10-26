#include "level_loader.h"

#include <cstdio>
#include <fstream>
#include <unordered_map>
#include <utility>
#include <json.hpp>

#include "logger.h"
#include "level.h"
#include "game.h"

namespace
{

const std::unordered_map<LevelId, std::string> level_filename =
{
  { LevelId::MAIN_LEVEL, "media/mainlevel.json" },
  { LevelId::LEVEL_ONE,  "media/level1.json" },
};

}

namespace LevelLoader
{

std::unique_ptr<Level> load_level(LevelId level_id)
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
  if (level_json.count("BackgroundId") == 0 || !level_json["BackgroundId"].is_number())
  {
    LOG_CRITICAL("Level is missing \"BackgroundId\" attribute or wrong type!");
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
  const auto background_id = level_json["BackgroundId"].get<int>();
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
                                   background_id,
                                   std::move(tile_ids),
                                   std::move(item_ids),
                                   std::vector<MovingPlatform>(
                                   {
                                     {
                                       // Vertical
                                       geometry::Position(38 * 16,  7 * 16),
                                       geometry::Position(38 * 16, 22 * 16),
                                       2,
                                       616,
                                       4
                                     },
                                     {
                                       // Horizontal
                                       geometry::Position(11 * 16,  8 * 16),
                                       geometry::Position( 7 * 16,  8 * 16),
                                       2,
                                       612,
                                       4
                                     }
                                   }));
  }
  else if (level_id == LevelId::LEVEL_ONE)
  {
    return std::make_unique<Level>(level_id,
                                   width,
                                   height,
                                   geometry::Position(4 * 16, 22 * 16),  // Player spawn
                                   background_id,
                                   std::move(tile_ids),
                                   std::move(item_ids),
                                   std::vector<MovingPlatform>(
                                   {
                                     {
                                       geometry::Position(36 * 16,  7 * 16),
                                       geometry::Position(36 * 16, 22 * 16),
                                       2,
                                       616,
                                       4
                                     },
                                   }));
  }
  else
  {
    return std::unique_ptr<Level>();
  }
}

}
