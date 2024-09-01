// TODO: refactor and combine with Enemy
#pragma once
#include <utility>

#include "geometry.h"
#include "misc.h"
#include "sprite.h"

class Hazard
{
 public:
  Hazard(geometry::Position position) : position(position), detection_rect_() {}
  virtual ~Hazard() = default;

  virtual void update(const geometry::Rectangle& player_rect) = 0;
  virtual Sprite get_sprite() const = 0;
  const geometry::Rectangle& get_detection_rect() const { return detection_rect_; }

  geometry::Position position;

 protected:
  geometry::Rectangle detection_rect_;
};

class AirTank : public Hazard
{
  // Die if shot
 public:
  AirTank(geometry::Position position, bool top) : Hazard(position), top_(top) {}

  virtual void update(const geometry::Rectangle& player_rect) override;
  virtual Sprite get_sprite() const override;

 private:
  bool top_;
  int frame_ = 0;
};

class Laser : public Hazard
{
  // Faces left/right, fires slow laser at player when they enter line
 public:
  Laser(geometry::Position position, bool left) : Hazard(position), left_(left) {}

  virtual void update(const geometry::Rectangle& player_rect) override;
  virtual Sprite get_sprite() const override { return left_ ? Sprite::SPRITE_LASER_L : Sprite::SPRITE_LASER_R; }

 private:
  bool left_;
};

class Thorn : public Hazard
{
  // Thrusts up when player is above
 public:
  Thorn(geometry::Position position) : Hazard(position) { detection_rect_ = geometry::Rectangle(position.x(), 0, 16, position.y() + 16); }

  virtual void update(const geometry::Rectangle& player_rect) override;
  virtual Sprite get_sprite() const override { return static_cast<Sprite>(static_cast<int>(Sprite::SPRITE_THORN_1) + frame_); }

 private:
  int frame_ = 0;
};
