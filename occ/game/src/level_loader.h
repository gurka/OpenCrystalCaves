#ifndef LEVEL_LOADER_H_
#define LEVEL_LOADER_H_

#include <string>
#include <memory>
#include <vector>

class LevelBase;
class Item;

namespace LevelLoader
{

std::unique_ptr<LevelBase> load_level(const std::string& filename);

}

#endif  // LEVEL_LOADER_H_
