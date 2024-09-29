#include "particle.h"

constexpr decltype(Explosion::sprites_) Explosion::sprites_;

void Explosion::update()
{
  if (is_alive())
  {
    frame_++;
  }
}
int Explosion::get_sprite() const
{
	return sprites_[frame_];
}
bool Explosion::is_alive() const
{
  return frame_ < sprites_.size();
}
