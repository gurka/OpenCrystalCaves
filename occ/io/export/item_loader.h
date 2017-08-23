#ifndef ITEMLOADER_H_
#define ITEMLOADER_H_

#include <string>
#include <vector>

#include "item.h"

class ItemLoader
{
 public:
  ItemLoader() = delete;

  static std::vector<Item> load_items(const std::string& filename);
};

#endif  // ITEMLOADER_H_
