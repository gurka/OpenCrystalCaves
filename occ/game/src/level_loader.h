#ifndef LEVEL_LOADER_H_
#define LEVEL_LOADER_H_

#include <string>
#include <memory>
#include <vector>

class LevelImpl;
class Item;

namespace LevelLoader
{

std::unique_ptr<LevelImpl> load_level(const std::string& filename);

}

#endif  // LEVEL_LOADER_H_
