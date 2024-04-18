#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include "game.h"

// TODO: Refactor/rename Object, ObjectManager, Background, Tile, Item, LevelObject, ...
class ObjectManager
{
 public:
  ObjectManager() : backgrounds_(), tiles_(), items_() {}

  bool load(const std::string& filename);

  const Background& get_background(const std::string& bg) const { return backgrounds_.at(bg); }
  const Tile& get_tile(int id) const { return tiles_[id]; }
  const Item& get_item(int id) const { return items_[id]; }

 private:
  std::unordered_map<std::string, Background> backgrounds_;
  std::vector<Tile> tiles_;
  std::vector<Item> items_;
};
