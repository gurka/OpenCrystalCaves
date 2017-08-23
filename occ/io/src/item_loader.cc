#include "item_loader.h"

#include <fstream>
#include <limits>

#include "json.hpp"
#include "logger.h"

std::vector<Item> ItemLoader::load_items(const std::string& filename)
{
  std::vector<Item> items;

  // Open and parse JSON file
  std::ifstream items_file(filename);
  if (!items_file.good())
  {
    LOG_CRITICAL("Could not open file \"%s\"", filename.c_str());
    return std::vector<Item>();
  }
  nlohmann::json items_json;
  items_file >> items_json;
  items_file.close();

  // Verify that it's an array
  if (!items_json.is_array())
  {
    LOG_CRITICAL("JSON is not an array!");
    return std::vector<Item>();
  }

  // Iterate over each item
  for (const auto& item_json : items_json)
  {
    // Verify that the required attributes exists
    if (item_json.count("Name") == 0 || !item_json["Name"].is_string())
    {
      LOG_CRITICAL("Item is missing \"Name\" attribute or wrong type!");
      return std::vector<Item>();
    }

    if (item_json.count("Sprite") == 0 || !item_json["Sprite"].is_number())
    {
      LOG_CRITICAL("Item is missing \"Sprite\" attribute or wrong type!");
      return std::vector<Item>();
    }

    if (item_json.count("SpriteCount") == 0 || !item_json["SpriteCount"].is_number())
    {
      LOG_CRITICAL("Item is missing \"SpriteCount\" attribute or wrong type!");
      return std::vector<Item>();
    }

    if (item_json.count("Type") == 0 || !item_json["Type"].is_number())
    {
      LOG_CRITICAL("Item is missing \"Type\" attribute or wrong type!");
      return std::vector<Item>();
    }

    if (item_json.count("Flags") == 0 || !item_json["Flags"].is_number())
    {
      LOG_CRITICAL("Item is missing \"Flags\" attribute or wrong type!");
      return std::vector<Item>();
    }

    // Create Item and read required attributes
    auto name = item_json["Name"].get<std::string>();
    auto sprite = item_json["Sprite"].get<int>();
    auto sprite_count = item_json["SpriteCount"].get<int>();
    auto type = item_json["Type"].get<int>();
    auto flags = item_json["Flags"].get<int>();


    // Debug
    LOG_DEBUG("Item: %s Sprite: %d SpriteCount: %d Type: %d Flags: 0x%x",
              name.c_str(),
              sprite,
              sprite_count,
              type,
              flags);

    // Add the item
    items.emplace_back(name, sprite, sprite_count, type, flags);
  }

  LOG_INFO("Loaded %d items", static_cast<int>(items.size()));

  return items;
}
