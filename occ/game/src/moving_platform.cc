#include "moving_platform.h"

#include "level.h"

MovingPlatform::MovingPlatform(geometry::Position position, const bool horizontal, const bool moving)
  : position(position),
    moving(moving),
    sprite_id(static_cast<int>(horizontal ? Sprite::SPRITE_PLATFORM_H_1 : Sprite::SPRITE_PLATFORM_V_1)),
    num_sprites(4),
    velocity(horizontal ? Vector<int>(1, 0) * 2 : Vector<int>(0, 1) * 2)
{
}

void MovingPlatform::update(const Level& level)
{
  if (moving)
  {
    // Move platform
    const auto new_platform_pos = collide_position() + velocity;

    if (level.collides_solid(new_platform_pos, collide_size))
    {
      // Change direction
      velocity = -velocity;
    }
    position += velocity;
  }
}
