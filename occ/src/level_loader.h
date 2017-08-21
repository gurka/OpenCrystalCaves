#ifndef LEVELLOADER_H_
#define LEVELLOADER_H_

#include <string>

#include "level.h"
#include "json.hpp"

class LevelLoader
{
 public:
  Level load_level(const std::string& filename);
};

#endif  // LEVELLOADER_H_
