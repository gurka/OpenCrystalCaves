#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include "game.h"

class ObjectManager
{
 public:
  ObjectManager() : backgrounds_() {}

  bool load(const std::string& filename);

  const Background& get_background(const std::string& bg) const { return backgrounds_.at(bg); }

 private:
  std::unordered_map<std::string, Background> backgrounds_;
};
