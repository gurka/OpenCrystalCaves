#include "enemy.h"

#include "hazard.h"
#include "level.h"

// TODO: hurt player on touch, all enemy types

bool Enemy::should_reverse(const Level& level) const
{
  // Reverse direction if colliding left/right or about to fall
  // Note: falling looks at two points near the left- and right- bottom corners
  return level.collides_solid(position, size) || !level.collides_solid(position + geometry::Position(1, 1), geometry::Size(1, size.y())) ||
    !level.collides_solid(position + geometry::Position(size.x() - 1, 1), geometry::Size(1, size.y()));
}

void Bigfoot::update(const geometry::Rectangle& player_rect, Level& level)
{
  frame_++;
  if (frame_ == 8)
  {
    frame_ = 0;
  }
  const int speed = running_ ? 4 : 2;
  const auto d = geometry::Position(left_ ? -speed : speed, 0);
  position += d;
  if (should_reverse(level))
  {
    left_ = !left_;
    running_ = false;
    position -= d;
  }
  // detect player/run
  if (geometry::is_any_colliding(get_detection_rects(level), player_rect))
  {
    running_ = true;
  }
}

std::vector<std::pair<geometry::Position, Sprite>> Bigfoot::get_sprites() const
{
  Sprite s = Sprite::SPRITE_BIGFOOT_HEAD_R_1;
  if (left_)
  {
    s = Sprite::SPRITE_BIGFOOT_HEAD_L_1;
  }
  const auto frame = running_ ? frame_ % 4 : frame_ / 2;
  return {
    std::make_pair(position, static_cast<Sprite>(static_cast<int>(s) + frame)),
    std::make_pair(position + geometry::Position(0, 16), static_cast<Sprite>(static_cast<int>(s) + 4 + frame)),
  };
}

void Hopper::update([[maybe_unused]] const geometry::Rectangle& player_rect, Level& level)
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
  const auto d = geometry::Position(left_ ? -4 : 4, 0);
  position += d;
  if (next_reverse_ == 0 || should_reverse(level))
  {
    left_ = !left_;
    position -= d;
    // Change directions every 1-20 seconds
    next_reverse_ = 17 * (1 + static_cast<int>(rand() % 19));
  }
  next_reverse_--;
}

std::vector<std::pair<geometry::Position, Sprite>> Hopper::get_sprites() const
{
  return {std::make_pair(position, static_cast<Sprite>(static_cast<int>(Sprite::SPRITE_HOPPER_1) + frame_))};
}

void Slime::update([[maybe_unused]] const geometry::Rectangle& player_rect, [[maybe_unused]] Level& level)
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

void Snake::update([[maybe_unused]] const geometry::Rectangle& player_rect, Level& level)
{
  // State changes / pause
  frame_++;
  if (paused_)
  {
    if (frame_ == 14)
    {
      paused_ = false;
      frame_ = 0;
    }
  }
  else if (frame_ == 100)
  {
    paused_ = true;
    frame_ = 0;
  }

  if (!paused_)
  {
    const auto d = geometry::Position(left_ ? -2 : 2, 0);
    position += d;
    if (should_reverse(level))
    {
      left_ = !left_;
      position -= d;
    }
  }
}

std::vector<std::pair<geometry::Position, Sprite>> Snake::get_sprites() const
{
  const auto s = paused_ ? Sprite::SPRITE_SNAKE_PAUSE_1 : (left_ ? Sprite::SPRITE_SNAKE_WALK_L_1 : Sprite::SPRITE_SNAKE_WALK_R_1);
  const int frame = frame_ % (paused_ ? 7 : 9);
  return {std::make_pair(position, static_cast<Sprite>(static_cast<int>(s) + frame))};
}

void Snake::on_death(Level& level)
{
  // Create a corpse
  level.hazards.emplace_back(new CorpseSlime(position, Sprite::SPRITE_SNAKE_SLIME));
  // TODO: authentic mode, align corpse to tile coord
}

void Spider::update([[maybe_unused]] const geometry::Rectangle& player_rect, [[maybe_unused]] Level& level)
{
  frame_++;
  if (frame_ == 8)
  {
    frame_ = 0;
  }
  const auto d = geometry::Position(0, up_ ? -2 : 2);
  position += d;
  if (level.collides_solid(position, size))
  {
    up_ = !up_;
    position -= d;
  }
  // fire webs
  if (child_ == nullptr && geometry::is_any_colliding(get_detection_rects(level), player_rect))
  {
    child_ = new SpiderWeb(position, *this);
    level.hazards.emplace_back(child_);
  }
}

std::vector<std::pair<geometry::Position, Sprite>> Spider::get_sprites() const
{
  return {std::make_pair(position,
                         static_cast<Sprite>(static_cast<int>(up_ ? Sprite::SPRITE_SPIDER_UP_1 : Sprite::SPRITE_SPIDER_DOWN_1) + frame_))};
}
