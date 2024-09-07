// TODO: refactor and combine with Enemy
#pragma once
#include <utility>

#include "geometry.h"
#include "misc.h"
#include "sprite.h"

struct Level;

class Hazard
{
 public:
  Hazard(geometry::Position position) : position(position), detection_rect_() {}
  virtual ~Hazard() = default;

  virtual void update(const geometry::Rectangle& player_rect, Level& level) = 0;
  virtual Sprite get_sprite() const = 0;
  const geometry::Rectangle& get_detection_rect() const { return detection_rect_; }
  virtual bool is_alive() const { return true; }

  geometry::Position position;

 protected:
  geometry::Rectangle detection_rect_;
};

class AirTank : public Hazard
{
  // ⬛⬜⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬜⬛
  // ⬜⬛⚪⚪⚪⚪⚪⚪⚪⚪⚪⚪⚪⚪⬛🪦
  // ⬜⬛⚪🟥🟥⚪⚪🟥🟥⚪🟥🟥🟥⚪⬛🪦
  // ⬜⬛🟥⚪⚪🟥⚪🟥🟥⚪🟥⚪⚪🟥⬛🪦
  // ⬜⬛🟥🟥🟥🟥⚪🟥🟥⚪🟥🟥🟥⚪⬛🪦
  // ⬜⬛🟥⚪⚪🟥⚪🟥🟥⚪🟥⚪🟥⚪⬛🪦
  // ⬜⬛🟥⚪⚪🟥⚪🟥🟥⚪🟥⚪⚪🟥⬛🪦
  // ⬜⬛⚪⚪⚪⚪⚪⚪⚪⚪⚪⚪⚪⚪⬛🪦
  // ⬜⬛⚪⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⚪⬛🪦
  // ⬜⬛⬛🟥🟥🟥🟨🟨🟨🟨🦚🦚🦚⬛⬛🪦
  // ⬜⬛⬛🟥🟥🟥🟨🟨🟨🟨🦚🦚🦚⬛⬛🪦
  // ⬜⬛⚪⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⚪⬛🪦
  // ⬜⬛⚪⚪⚪⚪⚪⚪⚪⬛⬜⬛⚪⚪⬛🪦
  // ⬜⬛⚪⚪⚪⚪⚪⚪⚪⬛⬜⬛⚪⚪⬛🪦
  // ⬜⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛🪦
  // ⬛🪦🪦🪦🪦🪦🪦🪦🪦🪦🪦🪦🪦🪦🪦⬛
  // Die if shot
 public:
  AirTank(geometry::Position position, bool top) : Hazard(position), top_(top) {}

  virtual void update(const geometry::Rectangle& player_rect, Level& level) override;
  virtual Sprite get_sprite() const override;

 private:
  bool top_;
  int frame_ = 0;
};

class LaserBeam;

class Laser : public Hazard
{
  // ⚫🩵🩵⚫⚫⚫⚫⚫⚫⚫⚫⚫⚫⚫⚫⚫
  // ⚫🔴🟥🩵🩵⚫⚫⚫⚫⚫⚫⚫⚫⚫⚫⚫
  // ⚫⚫🔴🟥⚪🩵⚫⚫⚫⚫⚫⚫⚫⚫⚫⚫
  // ⚫⚫🔴🟥⚪⚪🩵🩵⚫⚫⚫⚫⚫⚫⬜⚫
  // ⚫🔴🟥⚪⚪⚪⚪⚪⬜⬜⬜⬜⬜⚫📘⚫
  // ⚫🩵🩵⚪🩵⚪🩵⚪⚫⚪⚪⚪📘⚫📘⚫
  // ⚫🩵🩵🩵⚪🩵⚪🩵⚫⚪⚪⚪📘⚫📘⚫
  // ⚫🔴🟥⚪⚪⚪⚪⚪⚫🪦🪦🪦🪦⚫📘⚫
  // ⚫⚫🔴🟥⚪⚪🪦🪦⚫⚫⚫⚫⚫⚫🪦⚫
  // ⚫⚫🔴🟥⚪🪦⚫⚫⚫⚫⚫⚫⚫⚫⚫⚫
  // ⚫🔴🟥🪦🪦⚫⚫⚫⚫⚫⚫⚫⚫⚫⚫⚫
  // ⚫🪦🪦⚫⚫⚫⚫⚫⚫⚫⚫⚫⚫⚫⚫⚫
  // Faces left/right, fires slow laser at player when they enter line
 public:
  Laser(geometry::Position position, bool left) : Hazard(position), left_(left)
  {
    if (left)
    {
      detection_rect_ = geometry::Rectangle(position.x() - 16 * 20, position.y(), 16 * 20 + 2, 16);
    }
    else
    {
      detection_rect_ = geometry::Rectangle(position.x() + 14, position.y(), 16 * 20, 16);
    }
  }

  virtual void update(const geometry::Rectangle& player_rect, Level& level) override;
  virtual Sprite get_sprite() const override { return left_ ? Sprite::SPRITE_LASER_L : Sprite::SPRITE_LASER_R; }
  void remove_child() { child_ = nullptr; }

 private:
  bool left_;
  LaserBeam* child_ = nullptr;
};

class LaserBeam : public Hazard
{
  // 🟥⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛🟥
  // ⬛🟥🚨🚨⬛⬛🟥⬛🚨⬛🟥🟥🟥⬛🟥⬛
  // ⬛🚨🟥⬛🚨🟥⬛🚨⬛🚨⬛⬛⬛🟥⬛🚨
  // 🚨⬛⬛🟥🟥⬛🚨⬛🟥⬛🚨🚨⬛⬛🚨⬛
  // ⬛⬛⬛⬛⬛🚨⬛⬛⬛⬛⬛⬛🚨🚨⬛⬛
  // Moves left/right, disappear on collide or out of frame
 public:
  LaserBeam(geometry::Position position, bool left, Laser& parent) : Hazard(position), left_(left), parent_(parent) {}

  virtual void update(const geometry::Rectangle& player_rect, Level& level) override;
  virtual Sprite get_sprite() const override { return frame_ == 0 ? Sprite::SPRITE_LASER_BEAM_1 : Sprite::SPRITE_LASER_BEAM_2; }
  virtual bool is_alive() const override { return alive_; }

 private:
  bool left_;
  int frame_ = 0;
  Laser& parent_;
  bool alive_ = true;
};

class Thorn : public Hazard
{
  // ⬛⬛⬛⬛⬛⬛⬛🦚⬛⬛⬛⬛⬛⬛⬛⬛
  // ⬛⬛⬛⬛⬛⬛⬛🦚⬛⬛⬛⬛⬛⬛⬛⬛
  // ⬛⬛⬛⬛⬛⬛⬛🦚🦚⬛⬛⬛⬛⬛⬛⬛
  // ⬛⬛⬛⬛⬛⬛⬛🟩🦚⬛⬛⬛⬛⬛⬛⬛
  // ⬛⬛⬛⬛⬛⬛⬛🟩⬛⬛⬛⬛⬛⬛⬛⬛
  // ⬛⬛⬛⬛⬛⬛🟩🦚🦚⬛⬛⬛⬛⬛⬛⬛
  // ⬛⬛⬛⬛⬛⬛🟩🦚🦚⬛⬛⬛⬛⬛⬛⬛
  // ⬛⬛⬛⬛⬛⬛🦚🦚🦚🦚⬛⬛⬛⬛⬛⬛
  // ⬛⬛⬛⬛⬛⬛🦚🟩🦚🦚⬛⬛⬛⬛⬛⬛
  // ⬛⬛⬛⬛⬛🦚🟩🦚🦚⬛⬛⬛⬛⬛⬛⬛
  // Thrusts up when player is above
 public:
  Thorn(geometry::Position position) : Hazard(position)
  {
    detection_rect_ = geometry::Rectangle(position.x(), position.y() - 16 * 2, 16, 16 * 3);
  }

  virtual void update(const geometry::Rectangle& player_rect, Level& level) override;
  virtual Sprite get_sprite() const override { return static_cast<Sprite>(static_cast<int>(Sprite::SPRITE_THORN_1) + frame_); }

 private:
  int frame_ = 0;
};
