#include "entrance.h"

#include "sprite.h"

void Entrance::update()
{
  switch (state)
  {
    case EntranceState::CLOSED:
      if (counter > 0)
      {
        counter--;
      }
      break;
    case EntranceState::OPEN:
      if (counter < 2)
      {
        counter++;
      }
      break;
    default:
      break;
  }
}

int Entrance::get_sprite() const
{
  switch (state)
  {
    case EntranceState::COMPLETE:
      return static_cast<int>(Sprite::SPRITE_ENTRY_DONE);
    default:
      return static_cast<int>(Sprite::SPRITE_ENTRY_1) + counter;
  }
}
