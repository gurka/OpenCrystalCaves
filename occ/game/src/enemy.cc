#include "enemy.h"

#include "level.h"

void Bigfoot::update([[maybe_unused]] const Level& level)
{
  frame_++;
  if (frame_ == 4)
  {
    frame_ = 0;
  }
  // TODO: move, detect player/run
}

std::vector<std::pair<geometry::Position, Sprite>> Bigfoot::get_sprites() const
{
  Sprite s = Sprite::SPRITE_BIGFOOT_HEAD_R_1;
  if (left_)
  {
    s = Sprite::SPRITE_BIGFOOT_HEAD_L_1;
  }
  return {
    std::make_pair(position - geometry::Position(0, 16), static_cast<Sprite>(static_cast<int>(s) + frame_)),
    std::make_pair(position, static_cast<Sprite>(static_cast<int>(s) + 4 + frame_)),
  };
}

void Hopper::update(const Level& level)
{
  frame_ += left_ ? -1 : 1;
  if (frame_ == 18)
  {
    frame_ = 0;
  }
  else if (frame_ <= 0)
  {
    frame_ = 17;
  }
  if (left_)
  {
    position -= geometry::Position(4, 0);
  }
  else
  {
    position += geometry::Position(4, 0);
  }
  // Reverse direction if colliding left/right or about to fall
  // Note: falling looks at two points near the left- and right- bottom corners
  if (next_reverse_ == 0 || level.collides_solid(position, geometry::Size(16, 16)) ||
      !level.collides_solid(position + geometry::Position(1, 1), geometry::Size(1, 16)) ||
      !level.collides_solid(position + geometry::Position(16 - 1, 1), geometry::Size(1, 16)))
  {
    left_ = !left_;
    // Change directions every 1-20 seconds
    next_reverse_ = 17 * (1 + static_cast<int>(rand() % 19));
  }
  next_reverse_--;
}

std::vector<std::pair<geometry::Position, Sprite>> Hopper::get_sprites() const
{
  return {std::make_pair(position, static_cast<Sprite>(static_cast<int>(Sprite::SPRITE_HOPPER_1) + frame_))};
}

void Slime::update([[maybe_unused]] const Level& level)
{
  frame_++;
  if (frame_ == 4)
  {
    frame_ = 0;
  }
  // TODO: move, pause
}

std::vector<std::pair<geometry::Position, Sprite>> Slime::get_sprites() const
{
  Sprite s = Sprite::SPRITE_SLIME_R_1;
  if (dx_ == 1)
  {
    s = Sprite::SPRITE_SLIME_R_1;
  }
  else if (dx_ == -1)
  {
    s = Sprite::SPRITE_SLIME_L_1;
  }
  else if (dy_ == 1)
  {
    s = Sprite::SPRITE_SLIME_U_1;
  }
  else
  {
    s = Sprite::SPRITE_SLIME_D_1;
  }
  return {std::make_pair(position, static_cast<Sprite>(static_cast<int>(s) + frame_))};
}

void Snake::update([[maybe_unused]] const Level& level)
{
  frame_++;
  if (frame_ >= (paused_ ? 7 : 9))
  {
    frame_ = 0;
  }
  // TODO: move, pause
}

std::vector<std::pair<geometry::Position, Sprite>> Snake::get_sprites() const
{
  const auto s = paused_ ? Sprite::SPRITE_SNAKE_PAUSE_1 : (left_ ? Sprite::SPRITE_SNAKE_WALK_L_1 : Sprite::SPRITE_SNAKE_WALK_R_1);
  return {std::make_pair(position, static_cast<Sprite>(static_cast<int>(s) + frame_))};
}

void Spider::update([[maybe_unused]] const Level& level)
{
  frame_++;
  if (frame_ == 8)
  {
    frame_ = 0;
  }
  // TODO: move, fire webs
}

std::vector<std::pair<geometry::Position, Sprite>> Spider::get_sprites() const
{
  return {std::make_pair(position,
                         static_cast<Sprite>(static_cast<int>(up_ ? Sprite::SPRITE_SPIDER_UP_1 : Sprite::SPRITE_SPIDER_DOWN_1) + frame_))};
}
