#include "level_loader.h"

#include <cstdio>
#include <fstream>
#include <utility>
#include <json.hpp>

#include "logger.h"
#include "level_base.h"
#include "level_mainlevel.h"
#include "level_one.h"

namespace LevelLoader
{

std::unique_ptr<LevelBase> load_level(const std::string& filename)
{
  // Open and parse JSON file
  std::ifstream level_file(filename);
  if (!level_file.good())
  {
    LOG_CRITICAL("Could not open file \"%s\"", filename.c_str());
    return std::unique_ptr<LevelBase>();
  }
  nlohmann::json level_json;
  level_file >> level_json;
  level_file.close();

  // Check and get "Width"
  if (level_json.count("Width") == 0 || !level_json["Width"].is_number())
  {
    LOG_CRITICAL("Level is missing \"Width\" attribute or wrong type!");
    return std::unique_ptr<LevelBase>();
  }
  auto width = level_json["Width"].get<int>();

  // Check and get "Height"
  if (level_json.count("Height") == 0 || !level_json["Height"].is_number())
  {
    LOG_CRITICAL("Level is missing \"Height\" attribute or wrong type!");
    return std::unique_ptr<LevelBase>();
  }
  auto height = level_json["Height"].get<int>();

  // Check and get "Items"
  if (level_json.count("Items") == 0 || !level_json["Items"].is_object())
  {
    LOG_CRITICAL("Level is missing \"Items\" attribute or wrong type!");
    return std::unique_ptr<LevelBase>();
  }
  const auto& items_json = level_json["Items"];

  // Check and get "Foreground"
  if (items_json.count("Foreground") == 0 || !items_json["Foreground"].is_array())
  {
    LOG_CRITICAL("Items is missing \"Foreground\" attribute or wrong type!");
    return std::unique_ptr<LevelBase>();
  }
  auto items_foreground = items_json["Foreground"].get<std::vector<Item::Id>>();

  // Check and get "Score"
  if (items_json.count("Score") == 0 || !items_json["Score"].is_array())
  {
    LOG_CRITICAL("Items is missing \"Score\" attribute or wrong type!");
    return std::unique_ptr<LevelBase>();
  }
  auto items_score = items_json["Score"].get<std::vector<Item::Id>>();

  LOG_INFO("Loaded level from '%s'", filename.c_str());
  LOG_DEBUG("Level information: width=%d height=%d", width, height);

  // TODO: Would be nice if we could verify that the given level is a valid
  // level before we get this far (without having to specify all valid levels in multiple places...)
  if (filename == "media/mainlevel.json")
  {
    return std::make_unique<LevelMainLevel>(width, height, std::move(items_foreground), std::move(items_score));
  }
  else if (filename == "media/level1.json")
  {
    return std::make_unique<LevelOne>(width, height, std::move(items_foreground), std::move(items_score));
  }
  else
  {
    return std::unique_ptr<LevelBase>();
  }
}

}
