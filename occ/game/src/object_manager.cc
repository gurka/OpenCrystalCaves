#include <fstream>
#include <limits>
#include <json.hpp>

#include "logger.h"
#include "object_manager.h"

bool ObjectManager::load(const std::string& filename)
{
  // Check if objects already loaded
  if (!backgrounds_.empty() || !tiles_.empty() || !items_.empty())
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
  if (objects_json.count("Backgrounds") == 0 || !objects_json["Backgrounds"].is_array())
  {
    LOG_CRITICAL("JSON missing \"Backgrounds\" attribute or wrong type!");
    return false;
  }

  for (const auto& background_json : objects_json["Backgrounds"])
  {
    // Verify that the required attributes exists
    if (background_json.count("Sprite") == 0 || !background_json["Sprite"].is_number())
    {
      LOG_CRITICAL("Background is missing \"Sprite\" attribute or wrong type!");
      return false;
    }

    if (background_json.count("SpriteWidth") == 0 || !background_json["SpriteWidth"].is_number())
    {
      LOG_CRITICAL("Background is missing \"SpriteWidth\" attribute or wrong type!");
      return false;
    }

    if (background_json.count("SpriteHeight") == 0 || !background_json["SpriteHeight"].is_number())
    {
      LOG_CRITICAL("Background is missing \"SpriteHeight\" attribute or wrong type!");
      return false;
    }

    // Create and add Background
    backgrounds_.emplace_back(background_json["Sprite"].get<int>(),
                              geometry::Size(background_json["SpriteWidth"].get<int>(),
                                             background_json["SpriteHeight"].get<int>()));

    LOG_DEBUG("Loaded background: Sprite: %d SpriteWidth: %d SpriteHeight: %d",
              background_json["Sprite"].get<int>(),
              background_json["SpriteWidth"].get<int>(),
              background_json["SpriteHeight"].get<int>());
  }

  LOG_INFO("Loaded %d backgrounds", static_cast<int>(backgrounds_.size()));

  // Parse Tiles
  if (objects_json.count("Tiles") == 0 || !objects_json["Tiles"].is_array())
  {
    LOG_CRITICAL("JSON missing \"Tiles\" attribute or wrong type!");
    return false;
  }

  for (const auto& tiles_json : objects_json["Tiles"])
  {
    // Verify that the required attributes exists
    if (tiles_json.count("Sprite") == 0 || !tiles_json["Sprite"].is_number())
    {
      LOG_CRITICAL("Tile is missing \"Sprite\" attribute or wrong type!");
      return false;
    }

    if (tiles_json.count("SpriteCount") == 0 || !tiles_json["SpriteCount"].is_number())
    {
      LOG_CRITICAL("Tile is missing \"SpriteCount\" attribute or wrong type!");
      return false;
    }

    if (tiles_json.count("Flags") == 0 || !tiles_json["Flags"].is_number())
    {
      LOG_CRITICAL("Tile is missing \"Flags\" attribute or wrong type!");
      return false;
    }

    // Create and add Tiles
    tiles_.emplace_back(tiles_json["Sprite"].get<int>(),
                        tiles_json["SpriteCount"].get<int>(),
                        tiles_json["Flags"].get<int>());

    LOG_DEBUG("Loaded tile: Sprite: %d SpriteCount: %d Flags: %d",
              tiles_json["Sprite"].get<int>(),
              tiles_json["SpriteCount"].get<int>(),
              tiles_json["Flags"].get<int>());
  }

  LOG_INFO("Loaded %d tiles", static_cast<int>(tiles_.size()));

  // Parse Items
  if (objects_json.count("Items") == 0 || !objects_json["Items"].is_array())
  {
    LOG_CRITICAL("JSON missing \"Items\" attribute or wrong type!");
    return false;
  }

  for (const auto& items_json : objects_json["Items"])
  {
    // Verify that the required attributes exists
    if (items_json.count("Sprite") == 0 || !items_json["Sprite"].is_number())
    {
      LOG_CRITICAL("Item is missing \"Sprite\" attribute or wrong type!");
      return false;
    }

    if (items_json.count("Type") == 0 || !items_json["Type"].is_number())
    {
      LOG_CRITICAL("Item is missing \"Type\" attribute or wrong type!");
      return false;
    }

    if (items_json.count("Amount") == 0 || !items_json["Amount"].is_number())
    {
      LOG_CRITICAL("Item is missing \"Amount\" attribute or wrong type!");
      return false;
    }

    // Create and add Items
    items_.emplace_back(items_json["Sprite"].get<int>(),
                        items_json["Type"].get<int>(),
                        items_json["Amount"].get<int>());

    LOG_DEBUG("Loaded item: Sprite: %d Type: %d Amount: %d",
              items_json["Sprite"].get<int>(),
              items_json["Type"].get<int>(),
              items_json["Amount"].get<int>());
  }

  LOG_INFO("Loaded %d items", static_cast<int>(items_.size()));

  return true;
}
