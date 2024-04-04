#pragma once

#include "geometry.h"

class Background
{
 public:
  Background() : valid_(false), sprite_id_(-1), size_in_tiles_(0, 0) {}

  Background(int sprite_id, geometry::Size size_in_tiles) : valid_(true), sprite_id_(sprite_id), size_in_tiles_(size_in_tiles) {}

  bool valid() const { return valid_; }
  int get_sprite() const { return sprite_id_; }
  int get_sprite(const int x, const int y) const { return get_sprite() + (((y + 1) % get_size().y()) * 4) + (x % get_size().x()); }
  const geometry::Size& get_size() const { return size_in_tiles_; }

  static const Background INVALID;

 private:
  bool valid_;
  int sprite_id_;
  geometry::Size size_in_tiles_;
};
