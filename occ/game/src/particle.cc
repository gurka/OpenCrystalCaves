#include "particle.h"

ScoreParticle::ScoreParticle(geometry::Position position, int score) : Particle(position)
{
  switch (score)
  {
    case 100:
      sprite_ = Sprite::SPRITE_100;
      break;
    case 200:
      sprite_ = Sprite::SPRITE_200;
      break;
    case 400:
      sprite_ = Sprite::SPRITE_400;
      break;
    case 500:
      sprite_ = Sprite::SPRITE_500;
      break;
    case 800:
      sprite_ = Sprite::SPRITE_800;
      break;
    case 1000:
      sprite_ = Sprite::SPRITE_1000;
      break;
    case 2000:
      sprite_ = Sprite::SPRITE_2000;
      break;
    case 5000:
      sprite_ = Sprite::SPRITE_5000;
      break;
    case 10000:
      sprite_ = Sprite::SPRITE_10K;
      break;
    default:
      sprite_ = Sprite::SPRITE_NONE;
      break;
  }
}

void ScoreParticle::update()
{
  frame_++;
  position += geometry::Position(0, -1);
}
int ScoreParticle::get_sprite() const
{
  return static_cast<int>(sprite_);
}
bool ScoreParticle::is_alive() const
{
  return frame_ < 16;
}

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
