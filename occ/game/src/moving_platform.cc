#include "moving_platform.h"

#include "level.h"

MovingPlatform::MovingPlatform(geometry::Position position, const bool horizontal)
  : position(position),
    velocity(horizontal ? Vector<int>(1, 0) * 2 : Vector<int>(0, 1) * 2),
    sprite_id(static_cast<int>(horizontal ? Sprite::SPRITE_PLATFORM_H_1 : Sprite::SPRITE_PLATFORM_V_1)),
    num_sprites(4)
{
}
