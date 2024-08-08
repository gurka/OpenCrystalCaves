#include "enemy.h"

void Spider::update()
{
  frame_++;
  if (frame_ == 8)
  {
    frame_ = 0;
  }
  // TODO: fire webs
}

Sprite Spider::get_sprite() const
{
  return static_cast<Sprite>(static_cast<int>(up_ ? Sprite::SPRITE_SPIDER_UP_1 : Sprite::SPRITE_SPIDER_DOWN_1) + frame_);
}
