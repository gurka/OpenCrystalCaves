#include "hazard.h"

#include "level.h"

void AirTank::update([[maybe_unused]] const geometry::Rectangle& player_rect, [[maybe_unused]] Level& level)
{
  // TODO: check if shot
  frame_++;
  if (frame_ == 2)
  {
    frame_ = 0;
  }
}

Sprite AirTank::get_sprite() const
{
  if (top_)
  {
    return static_cast<Sprite>(static_cast<int>(Sprite::SPRITE_AIR_TANK_TOP_1) + frame_);
  }
  return Sprite::SPRITE_AIR_TANK_BOTTOM;
}

void Laser::update(const geometry::Rectangle& player_rect, Level& level)
{
  if (child_ == nullptr && isColliding(detection_rect_, player_rect))
  {
    geometry::Position child_pos = position + geometry::Position(left_ ? -12 : 12, -1);
    child_ = new LaserBeam(child_pos, left_, *this);
    level.hazards.emplace_back(child_);
  }
}

void LaserBeam::update([[maybe_unused]] const geometry::Rectangle& player_rect, Level& level)
{
  frame_ = 1 - frame_;
  position += geometry::Position(left_ ? -4 : 4, 0);
  if (level.collides_solid(position + geometry::Position(0, 1), geometry::Size(16, 16)))
  {
    alive_ = false;
    parent_.remove_child();
  }
}

void Thorn::update(const geometry::Rectangle& player_rect, [[maybe_unused]] Level& level)
{
  if (isColliding(detection_rect_, player_rect))
  {
    if (frame_ < 4)
    {
      frame_++;
    }
  }
  else
  {
    frame_ = 0;
  }
}
