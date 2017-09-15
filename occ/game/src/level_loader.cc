#include "level_loader.h"

#include <cstdio>
#include <fstream>
#include <utility>
#include <json.hpp>

#include "logger.h"
#include "level_impl.h"

namespace LevelLoader
{

std::unique_ptr<LevelImpl> load_level(const std::string& filename)
{
  // Open and parse JSON file
  std::ifstream level_file(filename);
  if (!level_file.good())
  {
    LOG_CRITICAL("Could not open file \"%s\"", filename.c_str());
    return std::unique_ptr<LevelImpl>();
  }
  nlohmann::json level_json;
  level_file >> level_json;
  level_file.close();

  // Check and get "Width"
  if (level_json.count("Width") == 0 || !level_json["Width"].is_number())
  {
    LOG_CRITICAL("Level is missing \"Width\" attribute or wrong type!");
    return std::unique_ptr<LevelImpl>();
  }
  auto width = level_json["Width"].get<int>();

  // Check and get "Height"
  if (level_json.count("Height") == 0 || !level_json["Height"].is_number())
  {
    LOG_CRITICAL("Level is missing \"Height\" attribute or wrong type!");
    return std::unique_ptr<LevelImpl>();
  }
  auto height = level_json["Height"].get<int>();

  // Check and get "Items"
  if (level_json.count("Items") == 0 || !level_json["Items"].is_object())
  {
    LOG_CRITICAL("Level is missing \"Items\" attribute or wrong type!");
    return std::unique_ptr<LevelImpl>();
  }
  const auto& items_json = level_json["Items"];

  // Check and get "Foreground"
  if (items_json.count("Foreground") == 0 || !items_json["Foreground"].is_array())
  {
    LOG_CRITICAL("Items is missing \"Foreground\" attribute or wrong type!");
    return std::unique_ptr<LevelImpl>();
  }
  auto items_foreground = items_json["Foreground"].get<std::vector<Item::Id>>();

  // Check and get "Score"
  if (items_json.count("Score") == 0 || !items_json["Score"].is_array())
  {
    LOG_CRITICAL("Items is missing \"Score\" attribute or wrong type!");
    return std::unique_ptr<LevelImpl>();
  }
  auto items_score = items_json["Score"].get<std::vector<Item::Id>>();

  LOG_INFO("Loaded level from '%s'", filename.c_str());
  LOG_DEBUG("Level information: width=%d height=%d", width, height);

  // TODO: Would be nice if we could verify that the given level is a valid
  // level before we get this far (without having to specify all valid levels in multiple places...)
  if (filename == "media/mainlevel.json")
  {
    return std::make_unique<LevelImpl>(width,
                                       height,
                                       geometry::Position(32, 48),  // Player spawn
                                       Background(20, geometry::Size(2, 2)),
                                       std::move(items_foreground),
                                       std::move(items_score),
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
  else if (filename == "media/level1.json")
  {
    return std::make_unique<LevelImpl>(width,
                                       height,
                                       geometry::Position(4 * 16, 22 * 16),  // Player spawn
                                       Background(970, geometry::Size(2, 2)),
                                       std::move(items_foreground),
                                       std::move(items_score),
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
    return std::unique_ptr<LevelImpl>();
  }
}

}
