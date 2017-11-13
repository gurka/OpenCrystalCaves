#ifndef OBJECT_MANAGER_H_
#define OBJECT_MANAGER_H_

#include <string>
#include <vector>

#include "game.h"

// TODO: Refactor/rename Object, ObjectManager, Background, Tile, Item, LevelObject, ...
class ObjectManager
{
 public:
  ObjectManager()
    : backgrounds_(),
      tiles_(),
      items_()
  {
  }

  bool load(const std::string& filename);

  const Background& get_background(int id) const { return backgrounds_[id]; }
  const Tile& get_tile(int id) const { return tiles_[id]; }
  const Item& get_item(int id) const { return items_[id]; }

 private:
  std::vector<Background> backgrounds_;
  std::vector<Tile> tiles_;
  std::vector<Item> items_;
};

#endif  // OBJECT_MANAGER_H_
