#include "exit.h"

void Exit::update()
{
  if (open && counter < 3)
  {
    counter++;
  }
}

std::vector<std::pair<geometry::Position, Sprite>> Exit::get_sprites() const
{
  return {
    {position, static_cast<Sprite>(static_cast<int>(Sprite::SPRITE_EXIT_TOP_LEFT_1) + counter)},
    {position + geometry::Position(0, 16), static_cast<Sprite>(static_cast<int>(Sprite::SPRITE_EXIT_BOTTOM_LEFT_1) + counter)},
  };
}
