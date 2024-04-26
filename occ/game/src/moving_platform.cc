#include "moving_platform.h"

#include "level.h"

MovingPlatform::MovingPlatform(geometry::Position position_start, geometry::Position position_end, const bool horizontal)
  : position(position_start),
    forward(true),
    position_start(position_start),
    position_end(position_end),
    velocity_forward(get_velocity(position_start, position_end, 2)),
    sprite_id(static_cast<int>(horizontal ? Sprite::SPRITE_PLATFORM_H_1 : Sprite::SPRITE_PLATFORM_V_1)),
    num_sprites(4)
{
}
