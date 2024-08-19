#include "hazard.h"

void AirTank::update()
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

void Laser::update()
{
  // TODO: check if player in line and there are no laser beams
}

void Thorn::update()
{
  // TODO: check if player above
}