// TODO: refactor and combine with Enemy
#pragma once
#include <utility>

#include "geometry.h"
#include "misc.h"
#include "sprite.h"

class Hazard
{
 public:
  Hazard(geometry::Position position) : position(position) {}
  virtual ~Hazard() = default;

  virtual void update() = 0;
  virtual Sprite get_sprite() const = 0;

  geometry::Position position;
};

class Laser : public Hazard
{
  // Faces left/right, fires slow laser at player when they enter line
 public:
  Laser(geometry::Position position, bool left) : Hazard(position), left_(left) {}

  virtual void update() override;
  virtual Sprite get_sprite() const override { return left_ ? Sprite::SPRITE_LASER_L : Sprite::SPRITE_LASER_R; }

 private:
  bool left_;
};
