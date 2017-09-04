#ifndef LEVEL_LOADER_H_
#define LEVEL_LOADER_H_

#include <string>
#include <memory>

class Level;

namespace LevelLoader
{

std::unique_ptr<Level> load_level(const std::string& filename);

}

#endif  // LEVEL_LOADER_H_
