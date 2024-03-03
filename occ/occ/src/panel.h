#pragma once

#include "spritemgr.h"

class Panel
{
 public:
  Panel(const std::string& ucsd);

  void update();

  void draw(const SpriteManager& sprite_manager) const;

 private:
  std::vector<std::wstring> strings_;
  geometry::Size size_;
  geometry::Position question_pos_ = {0, 0};
  geometry::Position sparkle_pos_ = {0, 0};
  unsigned ticks_ = 0;
};
