// Base class of enemies and hazards
#pragma once
#include <utility>

#include "geometry.h"
#include "misc.h"
#include "sprite.h"

struct Level;

class Actor
{
 public:
  Actor(geometry::Position position, geometry::Size size) : position(std::move(position)), size(std::move(size)) {}
  virtual ~Actor() = default;

  virtual bool is_alive() const { return true; }
  virtual int get_points() const { return 0; }

  virtual void update(const geometry::Rectangle& player_rect, Level& level) = 0;
  virtual std::vector<std::pair<geometry::Position, Sprite>> get_sprites() const = 0;
  virtual std::vector<geometry::Rectangle> get_detection_rects([[maybe_unused]] const Level& level) const { return {}; }

  geometry::Position position;
  geometry::Size size;

 protected:
  std::vector<geometry::Rectangle> create_detection_rects(const int dx, const int dy, const Level& level) const;
};
