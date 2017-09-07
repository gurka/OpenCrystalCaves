#include "level_one.h"

LevelOne::LevelOne(int width,
                   int height,
                   std::vector<Item::Id> tiles_background,
                   std::vector<Item::Id> tiles_foreground,
                   std::vector<geometry::Rectangle> aabbs,
                   std::vector<geometry::Position> platforms)
  : LevelBase(width,
              height,
              std::move(tiles_background),
              std::move(tiles_foreground),
              std::move(aabbs),
              std::move(platforms)),
     platform_()
{
  player_.position = geometry::Position(4 * 16, 22 * 16);
  player_.velocity = Vector<int>(0, 0);
  player_.direction = Player::Direction::right;
}

void LevelOne::update(unsigned game_tick)
{
  // Clear all objects and moving platforms
  objects_.clear();
  moving_platforms_.clear();

  // Update platform
  const auto player_on_vertical_platform = (player_.position.y() + player_.size.y() == platform_.position_y) &&
                                           (player_.position.x() < (38 * 16) + 16) &&
                                           (player_.position.x() + player_.size.x() > 38 * 16);

  if (player_on_vertical_platform)
  {
    // Only move player if not colliding with any static objects
    const auto new_player_pos = player_.position + (platform_.down ? geometry::Position(0, 2) : geometry::Position(0, -2));
    if (!collides(geometry::Rectangle(new_player_pos, player_.size)))
    {
      player_.position = new_player_pos;
    }
  }

  platform_.position_y += platform_.down ? 2 : -2;
  if (platform_.position_y == (platform_.down ? (22 * 16) : ((7 * 16) + 4)))
  {
    platform_.down = !platform_.down;
  }

  // Add object
  const auto platform_sprite_index = game_tick % 4;
  objects_.emplace_back(geometry::Position(36 * 16, platform_.position_y),
                        geometry::Size(16, 16),
                        616 + platform_sprite_index);

  // Add collision
  moving_platforms_.emplace_back(36 * 16, platform_.position_y);
}
