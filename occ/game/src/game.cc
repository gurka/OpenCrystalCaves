#include "game.h"

#include <cstdint>
#include <array>

#include "item_loader.h"
#include "level_loader.h"
#include "misc.h"

bool Game::init()
{
  items_ = ItemLoader::load_items("media/items.json");
  if (items_.empty())
  {
    return false;
  }

  level_ = LevelLoader::load_level("media/mainlevel.json");
  if (!level_.valid())
  {
    return false;
  }

  player_.position = geometry::Position(16, 16);
  player_.velocity = Vector<int>(0, 0);
  player_.direction = Player::Direction::right;

  return true;
}

void Game::update(const PlayerInput& player_input)
{
  static constexpr auto jump_velocity = misc::make_array<int>(-8, -8, -4, -4, -2, -2, -2, -2, 0, 2, 2, 2, 2, 4, 4);
  static constexpr std::size_t jump_velocity_fall_index = 9;

  ///////////////////////////////////////////////////////////////////
  //
  //  Calculate and set player velocity (ONLY velocity) based on input and player state
  //
  ///////////////////////////////////////////////////////////////////

  // Set y velocity
  if (player_.jumping && player_.jump_tick < jump_velocity.size())
  {
    // Player is jumping or falling after a jump
    player_.velocity = Vector<int>(player_.velocity.x(), jump_velocity[player_.jump_tick]);
  }
  else if (player_input.jump && !player_.jumping && !player_.falling)
  {
    // Player starts to jump
    player_.velocity = Vector<int>(player_.velocity.x(), jump_velocity.front());
  }
  else
  {
    // Just apply gravity
    player_.velocity = Vector<int>(player_.velocity.x(), 8);
  }

  // Set x velocity
  if ((player_input.left && player_input.right) ||
      (!player_input.left && !player_input.right))
  {
    // Set zero x velocity
    player_.velocity = Vector<int>(0, player_.velocity.y());
  }
  else if (player_input.left)
  {
    // First step is 2 pixels / tick, then 4 pixels / tick
    // TODO: Current implementation makes it possible to change direction and keep 4/-4 pixels / tick. OK?
    if (player_.velocity.x() == 0)
    {
      player_.velocity = Vector<int>(-2, player_.velocity.y());
    }
    else
    {
      player_.velocity = Vector<int>(-4, player_.velocity.y());
    }
  }
  else if (player_input.right)
  {
    // Same as above
    if (player_.velocity.x() == 0)
    {
      player_.velocity = Vector<int>(2, player_.velocity.y());
    }
    else
    {
      player_.velocity = Vector<int>(4, player_.velocity.y());
    }
  }

  ///////////////////////////////////////////////////////////////////
  //
  //  Move player towards destination and check for collision
  //
  ///////////////////////////////////////////////////////////////////

  player_collide_x_ = false;
  player_collide_y_ = false;
  const auto destination = player_.position + player_.velocity;

  auto collides = [this](const geometry::Rectangle& player_rect)
  {
    for (const auto& aabb : level_.get_aabbs())
    {
      if (geometry::isColliding(player_rect, aabb))
      {
        return true;
      }
    }
    return false;
  };

  const auto step_x = destination.x() > player_.position.x() ? 1 : -1;
  while (player_.position.x() != destination.x())
  {
    const geometry::Rectangle player_rect { player_.position + geometry::Position(step_x, 0), player_.size.x(), player_.size.y() };
    if (collides(player_rect))
    {
      player_collide_x_ = true;
      break;
    }
    player_.position += geometry::Position(step_x, 0);
  }

  const auto step_y = destination.y() > player_.position.y() ? 1 : -1;
  while (player_.position.y() != destination.y())
  {
    const geometry::Rectangle player_rect { player_.position + geometry::Position(0, step_y), player_.size.x(), player_.size.y() };
    if (collides(player_rect))
    {
      player_collide_y_ = true;
      break;
    }
    player_.position += geometry::Position(0, step_y);
  }


  ///////////////////////////////////////////////////////////////////
  //
  //  Set new player information based on the movement
  //
  ///////////////////////////////////////////////////////////////////

  // Walking
  bool walking = player_.velocity.x() != 0 && !player_collide_x_;
  if (!player_.walking && walking)
  {
    // Player started to move
    player_.walk_tick = 0u;
  }
  else if (player_.walking && walking)
  {
    // Player is still walking
    player_.walk_tick += 1u;
  }
  player_.walking = walking;

  // Falling
  player_.falling = player_.velocity.y() >= 8 && !player_collide_y_;

  // Jumping
  bool jumping = player_.velocity.y() < 8 && !player_collide_y_;
  if (!player_.jumping && jumping)
  {
    // Player started to jump
    player_.jump_tick = 0u;
  }
  else if (player_.jumping && jumping)
  {
    // Player is still jumping
    player_.jump_tick += 1;
  }
  else if (player_.jumping && !jumping && player_.velocity.y() < 0)
  {
    // Player was jumping but hit something on the way up
    // Continue the jump but skip to falling velocity
    player_.jump_tick = jump_velocity_fall_index;
    jumping = true;
  }
  player_.jumping = jumping;

  // Direction
  if (player_.velocity.x() < 0)
  {
    player_.direction = Player::Direction::left;
  }
  else if (player_.velocity.x() > 0)
  {
    player_.direction = Player::Direction::right;
  }

  // Shooting
  //
  // NOTES:
  // If have ammo:
  //   Cannot fire another shoot if a shoot is currently in the game (not hit something or gone outside the level)
  //   Shooting sprite stays until player moves (not counting gravity)
  // If no ammo:
  //   If standing still or jumping without moving left/right:
  //     Shooting sprite only shown while holding shoot button
  //   If walking or jumping while moving left/right
  //     Shooting sprite never shown
  if (player_input.shoot)
  {
  }
}
