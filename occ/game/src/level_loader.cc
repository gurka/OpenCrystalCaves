#include "level_loader.h"

#include <cstdio>
#include <fstream>
#include <utility>
#include <json.hpp>

#include "logger.h"
#include "level.h"
#include "level_mainlevel.h"

namespace LevelLoader
{

std::unique_ptr<Level> load_level(const std::string& filename)
{
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

  // Check and get "Width"
  if (level_json.count("Width") == 0 || !level_json["Width"].is_number())
  {
    LOG_CRITICAL("Level is missing \"Width\" attribute or wrong type!");
    return std::unique_ptr<Level>();
  }
  auto width = level_json["Width"].get<int>();

  // Check and get "Height"
  if (level_json.count("Height") == 0 || !level_json["Height"].is_number())
  {
    LOG_CRITICAL("Level is missing \"Height\" attribute or wrong type!");
    return std::unique_ptr<Level>();
  }
  auto height = level_json["Height"].get<int>();

  // Check and get "Items"
  if (level_json.count("Items") == 0 || !level_json["Items"].is_object())
  {
    LOG_CRITICAL("Level is missing \"Items\" attribute or wrong type!");
    return std::unique_ptr<Level>();
  }
  const auto& items_json = level_json["Items"];

  // Check and get "Background"
  if (items_json.count("Background") == 0 || !items_json["Background"].is_array())
  {
    LOG_CRITICAL("Items is missing \"Background\" attribute or wrong type!");
    return std::unique_ptr<Level>();
  }
  auto items_background = items_json["Background"].get<std::vector<Item::Id>>();

  // Check and get "Foreground"
  if (items_json.count("Foreground") == 0 || !items_json["Foreground"].is_array())
  {
    LOG_CRITICAL("Items is missing \"Foreground\" attribute or wrong type!");
    return std::unique_ptr<Level>();
  }
  auto items_foreground = items_json["Foreground"].get<std::vector<Item::Id>>();

  // Check and get "AABBs"
  if (level_json.count("AABBs") == 0 || !level_json["AABBs"].is_array())
  {
    LOG_CRITICAL("Level is missing \"AABBs\" attribute or wrong type!");
    return std::unique_ptr<Level>();
  }
  std::vector<geometry::Rectangle> aabbs;
  for (const auto& aabb_json : level_json["AABBs"])
  {
    if (aabb_json.count("X") == 0 || !aabb_json["X"].is_number())
    {
      LOG_CRITICAL("AABB is missing \"X\" attribute or wrong type!");
      return std::unique_ptr<Level>();
    }

    if (aabb_json.count("Y") == 0 || !aabb_json["Y"].is_number())
    {
      LOG_CRITICAL("AABB is missing \"Y\" attribute or wrong type!");
      return std::unique_ptr<Level>();
    }

    if (aabb_json.count("W") == 0 || !aabb_json["W"].is_number())
    {
      LOG_CRITICAL("AABB is missing \"W\" attribute or wrong type!");
      return std::unique_ptr<Level>();
    }

    if (aabb_json.count("H") == 0 || !aabb_json["H"].is_number())
    {
      LOG_CRITICAL("AABB is missing \"H\" attribute or wrong type!");
      return std::unique_ptr<Level>();
    }

    auto x = aabb_json["X"].get<int>();
    auto y = aabb_json["Y"].get<int>();
    auto w = aabb_json["W"].get<int>();
    auto h = aabb_json["H"].get<int>();

    // Note that the AABBs are in "tiles" in the file but in pixels in Level
    aabbs.emplace_back(x * 16, y * 16, w * 16, h * 16);
  }

  // Check and get "Platforms"
  if (level_json.count("Platforms") == 0 || !level_json["Platforms"].is_array())
  {
    LOG_CRITICAL("Level is missing \"Platforms\" attribute or wrong type!");
    return std::unique_ptr<Level>();
  }
  std::vector<geometry::Position> platforms;
  for (const auto& aabb_json : level_json["Platforms"])
  {
    if (aabb_json.count("X") == 0 || !aabb_json["X"].is_number())
    {
      LOG_CRITICAL("Platform is missing \"X\" attribute or wrong type!");
      return std::unique_ptr<Level>();
    }

    if (aabb_json.count("Y") == 0 || !aabb_json["Y"].is_number())
    {
      LOG_CRITICAL("Platform is missing \"Y\" attribute or wrong type!");
      return std::unique_ptr<Level>();
    }

    auto x = aabb_json["X"].get<int>();
    auto y = aabb_json["Y"].get<int>();

    // Note that the Platforms are in "tiles" in the file but in pixels in Level
    platforms.emplace_back(x * 16, y * 16);
  }

  LOG_INFO("Loaded level from '%s'", filename.c_str());

  // Debug
  LOG_DEBUG("Level information: width=%d height=%d no_aabbs=%d no_platforms=%d",
            width,
            height,
            static_cast<int>(aabbs.size()),
            static_cast<int>(platforms.size()));

  // TODO: Would be nice if we could verify that the given level is a valid
  // level before we get this far (without having to specify all valid levels in multiple places...)
  if (filename == "media/mainlevel.json")
  {
    return std::make_unique<LevelMainLevel>(width,
                                            height,
                                            std::move(items_background),
                                            std::move(items_foreground),
                                            std::move(aabbs),
                                            std::move(platforms));
  }
  else
  {
    return std::unique_ptr<Level>();
  }
}

}
