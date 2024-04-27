#include <fstream>
#include <limits>
#include <nlohmann/json.hpp>

#include "logger.h"
#include "object_manager.h"

bool ObjectManager::load(const std::string& filename)
{
  // Check if objects already loaded
  if (!backgrounds_.empty())
  {
    return false;
  }

  // Open and parse JSON file
  std::ifstream objects_file(filename);
  if (!objects_file.good())
  {
    LOG_CRITICAL("Could not open file \"%s\"", filename.c_str());
    return false;
  }
  nlohmann::json objects_json;
  objects_file >> objects_json;
  objects_file.close();

  // Parse backgrounds
  if (objects_json.count("Backgrounds") == 0 || !objects_json["Backgrounds"].is_object())
  {
    LOG_CRITICAL("JSON missing \"Backgrounds\" attribute or wrong type!");
    return false;
  }

  for (const auto& background_json : objects_json["Backgrounds"].items())
  {
    // Verify that the required attributes exists
    if (background_json.value().count("Sprite") == 0 || !background_json.value()["Sprite"].is_number())
    {
      LOG_CRITICAL("Background is missing \"Sprite\" attribute or wrong type!");
      return false;
    }

    if (background_json.value().count("SpriteWidth") == 0 || !background_json.value()["SpriteWidth"].is_number())
    {
      LOG_CRITICAL("Background is missing \"SpriteWidth\" attribute or wrong type!");
      return false;
    }

    if (background_json.value().count("SpriteHeight") == 0 || !background_json.value()["SpriteHeight"].is_number())
    {
      LOG_CRITICAL("Background is missing \"SpriteHeight\" attribute or wrong type!");
      return false;
    }

    // Create and add Background
    backgrounds_[background_json.key()] = {
      background_json.value()["Sprite"].get<int>(),
      geometry::Size(background_json.value()["SpriteWidth"].get<int>(), background_json.value()["SpriteHeight"].get<int>())};

    LOG_DEBUG("Loaded background: Sprite: %d SpriteWidth: %d SpriteHeight: %d",
              background_json.value()["Sprite"].get<int>(),
              background_json.value()["SpriteWidth"].get<int>(),
              background_json.value()["SpriteHeight"].get<int>());
  }

  LOG_INFO("Loaded %d backgrounds", static_cast<int>(backgrounds_.size()));

  return true;
}
