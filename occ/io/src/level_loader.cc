#include "level_loader.h"

#include <cstdio>
#include <fstream>
#include <utility>

Level LevelLoader::load_level(const std::string& filename)
{
  // Open and parse JSON file
  std::ifstream level_file(filename);
  if (!level_file.good())
  {
    fprintf(stderr, "Could not open file \"%s\"\n", filename.c_str());
    return Level();
  }
  nlohmann::json level_json;
  level_file >> level_json;
  level_file.close();

  // Check and get "Width"
  if (level_json.count("Width") == 0 || !level_json["Width"].is_number())
  {
    fprintf(stderr, "Level is missing \"Width\" attribute or wrong type!\n");
    return Level();
  }
  auto width = level_json["Width"].get<int>();

  // Check and get "Height"
  if (level_json.count("Height") == 0 || !level_json["Height"].is_number())
  {
    fprintf(stderr, "Level is missing \"Height\" attribute or wrong type!\n");
    return Level();
  }
  auto height = level_json["Height"].get<int>();

  // Check and get "Items"
  if (level_json.count("Items") == 0 || !level_json["Items"].is_object())
  {
    fprintf(stderr, "Level is missing \"Items\" attribute or wrong type!\n");
    return Level();
  }
  const auto& items_json = level_json["Items"];

  // Check and get "Background"
  if (items_json.count("Background") == 0 || !items_json["Background"].is_array())
  {
    fprintf(stderr, "Items is missing \"Background\" attribute or wrong type!\n");
    return Level();
  }
  auto items_background = items_json["Background"].get<std::vector<Item::Id>>();

  // Check and get "Middleground"
  if (items_json.count("Middleground") == 0 || !items_json["Middleground"].is_array())
  {
    fprintf(stderr, "Items is missing \"Middleground\" attribute or wrong type!\n");
    return Level();
  }
  auto items_middleground = items_json["Middleground"].get<std::vector<Item::Id>>();

  // Check and get "Foreground"
  if (items_json.count("Foreground") == 0 || !items_json["Foreground"].is_array())
  {
    fprintf(stderr, "Items is missing \"Foreground\" attribute or wrong type!\n");
    return Level();
  }
  auto items_foreground = items_json["Foreground"].get<std::vector<Item::Id>>();

  // Check and get "AABBs"
  if (level_json.count("AABBs") == 0 || !level_json["AABBs"].is_array())
  {
    fprintf(stderr, "Level is missing \"AABBs\" attribute or wrong type!\n");
    return Level();
  }
  std::vector<geometry::Rectangle> aabbs;
  for (const auto& aabb_json : level_json["AABBs"])
  {
    if (aabb_json.count("X") == 0 || !aabb_json["X"].is_number())
    {
      fprintf(stderr, "AABB is missing \"X\" attribute or wrong type!\n");
      return Level();
    }

    if (aabb_json.count("Y") == 0 || !aabb_json["Y"].is_number())
    {
      fprintf(stderr, "AABB is missing \"Y\" attribute or wrong type!\n");
      return Level();
    }

    if (aabb_json.count("W") == 0 || !aabb_json["W"].is_number())
    {
      fprintf(stderr, "AABB is missing \"W\" attribute or wrong type!\n");
      return Level();
    }

    if (aabb_json.count("H") == 0 || !aabb_json["H"].is_number())
    {
      fprintf(stderr, "AABB is missing \"H\" attribute or wrong type!\n");
      return Level();
    }

    auto x = aabb_json["X"].get<int>();
    auto y = aabb_json["Y"].get<int>();
    auto w = aabb_json["W"].get<int>();
    auto h = aabb_json["H"].get<int>();

    // Note that the AABBs are in "tiles" in the file but in pixels in Level
    aabbs.emplace_back(x * 16, y * 16, w * 16, h * 16);
  }

  // Debug
  fprintf(stderr, "Loaded level with width: %d height: %d\n", width, height);

  return Level(width,
               height,
               std::move(items_background),
               std::move(items_middleground),
               std::move(items_foreground),
               std::move(aabbs));
}
