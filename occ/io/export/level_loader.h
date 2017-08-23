#ifndef LEVELLOADER_H_
#define LEVELLOADER_H_

#include <string>

#include "level.h"

class LevelLoader
{
 public:
  LevelLoader() = delete;

  static Level load_level(const std::string& filename);
};

#endif  // LEVELLOADER_H_
