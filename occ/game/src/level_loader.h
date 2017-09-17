#ifndef LEVEL_LOADER_H_
#define LEVEL_LOADER_H_

#include <string>
#include <memory>
#include <vector>

enum class LevelId;
class Level;
class Item;

namespace LevelLoader
{

std::unique_ptr<Level> load_level(LevelId level_id);

}

#endif  // LEVEL_LOADER_H_
