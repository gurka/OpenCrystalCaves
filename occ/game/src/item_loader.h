#ifndef ITEM_LOADER_H_
#define ITEM_LOADER_H_

#include <string>
#include <vector>

#include "item.h"

namespace ItemLoader
{

std::vector<Item> load_items(const std::string& filename);

}

#endif  // ITEM_LOADER_H_
