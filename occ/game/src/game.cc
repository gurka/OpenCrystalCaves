#include "game.h"

#include <cstdint>
#include <array>

#include "item_loader.h"
#include "level_loader.h"
#include "misc.h"
#include "logger.h"

static constexpr auto gravity = 8u;
static constexpr auto jump_velocity = misc::make_array<int>(0, -8, -8, -8, -4, -4, -2, -2, -2, -2, 2, 2, 2, 2, 4, 4);
static constexpr auto jump_velocity_fall_index = 10u;

bool Game::init()
{
  items_ = ItemLoader::load_items("media/items.json");
  if (items_.empty())
  {
    return false;
  }

  //level_ = LevelLoader::load_level("media/mainlevel.json");
  level_ = LevelLoader::load_level("media/level1.json");
  if (!level_)
  {
    return false;
  }

  return true;
}

void Game::update(unsigned game_tick, const PlayerInput& player_input)
{
  // Update dynamic objects in the level
  level_->update(game_tick);

  // Currently the only thing that is alive within the game is the player :-(
  update_player(player_input);

  // But in the future we will have:
  // update_enemies()
  // update_objects()
  // update_xxxxxxx()
}

void Game::update_player(const PlayerInput& player_input)
{
  auto& player = level_->get_player();

  /**
   * Updating the player is done in these steps:
   * 1. Update player information based on input
   * 2. Update player velocity based on player information
   * 3. Update player position based on player velocity
   * 4. Update player information based on collision
   */


  /**
   * 1. Update player information based on input
   */

  // Check left / right
  if ((player_input.left && player_input.right) ||
      (!player_input.left && !player_input.right))
  {
    player.walking = false;
  }
  else if (player.walking &&
           ((player_input.right && player.direction == Player::Direction::right) ||
            (player_input.left  && player.direction == Player::Direction::left)))
  {
    // Player is still walking in the same direction, just update walk tick
    player.walk_tick += 1u;
  }
  else if (player_input.left || player_input.right)
  {
    // Player started to walk
    player.walking = true;
    player.walk_tick = 0u;

    // Set direction
    player.direction = player_input.right ? Player::Direction::right : Player::Direction::left;
  }

  // Check jump
  if (player_input.jump &&
      !player.jumping &&
      !player.falling &&
      !level_->collides(geometry::Rectangle(player.position + geometry::Position(0, -1),
                                            player.size)))
  {
    // Player wants to jump
    player.jumping = true;
    player.jump_tick = 0u;
  }
  else if (player.jumping)
  {
    // Player still jumping
    player.jump_tick += 1u;
  }

  // Check shooting
  player.shooting = player_input.shoot;

  /**
   * 2. Update player velocity based on player information
   */

  // Set y velocity
  if (player.jumping)
  {
    player.velocity = Vector<int>(player.velocity.x(), jump_velocity[player.jump_tick]);
  }
  else
  {
    player.velocity = Vector<int>(player.velocity.x(), gravity);
  }

  // Set x velocity
  if (player.walking)
  {
    // First step is 2 pixels / tick, then 4 pixels / tick
    const auto velocity = player.walk_tick == 0u ? 2 : 4;
    if (player.direction == Player::Direction::right)
    {
      player.velocity = Vector<int>(velocity, player.velocity.y());
    }
    else  // player.direction == Player::Direction::left
    {
      player.velocity = Vector<int>(-velocity, player.velocity.y());
    }
  }
  else
  {
    player.velocity = Vector<int>(0, player.velocity.y());
  }

  /**
   * 3. Update player position based on player velocity
   */

  player.collide_x = false;
  player.collide_y = false;
  const auto destination = player.position + player.velocity;

  // Move on x axis
  const auto step_x = destination.x() > player.position.x() ? 1 : -1;
  while (player.position.x() != destination.x())
  {
    const geometry::Rectangle player_rect { player.position + geometry::Position(step_x, 0), player.size.x(), player.size.y() };
    if (level_->collides(player_rect))
    {
      player.collide_x = true;
      break;
    }
    player.position += geometry::Position(step_x, 0);
  }

  // Move on y axis
  const auto step_y = destination.y() > player.position.y() ? 1 : -1;
  while (player.position.y() != destination.y())
  {
    const geometry::Rectangle player_rect { player.position + geometry::Position(0, step_y), player.size };
    if (level_->collides(player_rect))
    {
      player.collide_y = true;
      break;
    }

    // If player is falling down we need to check for collision with platforms
    if (step_y == 1)
    {
      const auto platform_collide = [this, &player_rect]()
      {
        for (const auto& platform : level_->get_platforms())
        {
          // Collision with platform only occurs if player is falling down (which we check above)
          // and only for one single pixel on y axis, when the player is exactly on top of the platform
          if ((player_rect.position.y() + player_rect.size.y() - 1 == platform.y()) &&
              (player_rect.position.x() < platform.x() + 16) &&
              (player_rect.position.x() + player_rect.size.x() > platform.x()))
          {
            return true;
          }
        }
        for (const auto& platform : level_->get_moving_platforms())
        {
          // Collision with platform only occurs if player is falling down (which we check above)
          // and only for one single pixel on y axis, when the player is exactly on top of the platform
          if ((player_rect.position.y() + player_rect.size.y() - 1 == platform.position.y()) &&
              (player_rect.position.x() < platform.position.x() + 16) &&
              (player_rect.position.x() + player_rect.size.x() > platform.position.x()))
          {
            return true;
          }
        }
        return false;
      }();
      if (platform_collide)
      {
        player.collide_y = true;
        break;
      }
    }
    player.position += geometry::Position(0, step_y);
  }

  /**
   * 4. Update player information based on collision
   */

  // Check if player hit something while walking
  if (player.walking && player.collide_x)
  {
    player.walking = false;
  }

  // Check if player still jumping
  if (player.jumping)
  {
    // Check if player hit something while jumping
    if (player.collide_y)
    {
      if (player.velocity.y() < 0)
      {
        // Player hit something while jumping up
        // Skip to "falling down" velocity
        player.jump_tick = jump_velocity_fall_index;
      }
      else  // player.velocity.y() > 0
      {
        // Player landed
        player.jumping = false;
      }
    }
    else if (player.jump_tick == jump_velocity.size() - 1u)
    {
      // Player jump ended
      player.jumping = false;
    }
    else if (player.jump_tick != 0 &&
             level_->collides(geometry::Rectangle(player.position + geometry::Position(0, 1),
                                                  player.size)))
    {
      // Player did not actually collide with the ground, but standing directly above it
      // and this isn't the first tick in the jump, so we can consider the jump to have
      // ended here
      player.jumping = false;
    }
  }

  // Check if player is falling
  player.falling = !player.jumping &&
                   player.velocity.y() > 0 &&
                   !player.collide_y &&
                   !level_->collides(geometry::Rectangle(player.position + geometry::Position(0, 1),
                                                         player.size));
}

