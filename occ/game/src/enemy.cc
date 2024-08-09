#include "enemy.h"

void Hopper::update()
{
  frame_++;
  if (frame_ == 18)
  {
    frame_ = 0;
  }
  // TODO: move, randomly change directions
}

Sprite Hopper::get_sprite() const
{
  return static_cast<Sprite>(static_cast<int>(Sprite::SPRITE_HOPPER_1) + frame_);
}


void Spider::update()
{
  frame_++;
  if (frame_ == 8)
  {
    frame_ = 0;
  }
  // TODO: move, fire webs
}

Sprite Spider::get_sprite() const
{
  return static_cast<Sprite>(static_cast<int>(up_ ? Sprite::SPRITE_SPIDER_UP_1 : Sprite::SPRITE_SPIDER_DOWN_1) + frame_);
}
