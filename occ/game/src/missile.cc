#include "missile.h"

constexpr decltype(Missile::size) Missile::size;
constexpr decltype(Missile::speed) Missile::speed;

bool Missile::is_in_cooldown() const
{
  return alive || cooldown > 0;
}

void Missile::set_cooldown()
{
  cooldown = 7;
}

void Missile::update()
{
  if (cooldown > 0)
  {
    cooldown--;
  }
  if (alive)
  {
    frame++;
    // TODO: Missile doesn't disappear after a certain number of frames, but rather
    //       when it's outside of the screen. So we might need to make GameImpl aware
    //       of the game camera...
    if (frame > 27)
    {
      alive = false;
    }
  }
}