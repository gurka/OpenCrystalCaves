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

  level_ = LevelLoader::load_level("media/mainlevel.json");
  if (!level_.valid())
  {
    return false;
  }

  player_.position = geometry::Position(32, 48);
  player_.velocity = Vector<int>(0, 0);
  player_.direction = Player::Direction::right;

  return true;
}

void Game::update(unsigned game_tick, const PlayerInput& player_input)
{
  // Update dynamic objects in the level
  level_.update(game_tick);

  // Currently the only thing that is alive within the game is the player :-(
  update_player(player_input);

  // But in the future we will have:
  // update_enemies()
  // update_objects()
  // update_xxxxxxx()
}

void Game::update_player(const PlayerInput& player_input)
{
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
    player_.walking = false;
  }
  else if (player_.walking &&
           ((player_input.right && player_.direction == Player::Direction::right) ||
            (player_input.left  && player_.direction == Player::Direction::left)))
  {
    // Player is still walking in the same direction, just update walk tick
    player_.walk_tick += 1u;
  }
  else if (player_input.left || player_input.right)
  {
    // Player started to walk
    player_.walking = true;
    player_.walk_tick = 0u;

    // Set direction
    player_.direction = player_input.right ? Player::Direction::right : Player::Direction::left;
  }

  // Check jump
  if (player_input.jump &&
      !player_.jumping &&
      !player_.falling &&
      !collides(geometry::Rectangle(player_.position + geometry::Position(0, -1),
                                    player_.size)))
  {
    // Player wants to jump
    player_.jumping = true;
    player_.jump_tick = 0u;
  }
  else if (player_.jumping)
  {
    // Player still jumping
    player_.jump_tick += 1u;
  }

  // Check shooting
  player_.shooting = player_input.shoot;

  /**
   * 2. Update player velocity based on player information
   */

  // Set y velocity
  if (player_.jumping)
  {
    player_.velocity = Vector<int>(player_.velocity.x(), jump_velocity[player_.jump_tick]);
  }
  else
  {
    player_.velocity = Vector<int>(player_.velocity.x(), gravity);
  }

  // Set x velocity
  if (player_.walking)
  {
    // First step is 2 pixels / tick, then 4 pixels / tick
    const auto velocity = player_.walk_tick == 0u ? 2 : 4;
    if (player_.direction == Player::Direction::right)
    {
      player_.velocity = Vector<int>(velocity, player_.velocity.y());
    }
    else  // player_.direction == Player::Direction::left
    {
      player_.velocity = Vector<int>(-velocity, player_.velocity.y());
    }
  }
  else
  {
    player_.velocity = Vector<int>(0, player_.velocity.y());
  }

  /**
   * 3. Update player position based on player velocity
   */

  player_.collide_x = false;
  player_.collide_y = false;
  const auto destination = player_.position + player_.velocity;

  // Move on x axis
  const auto step_x = destination.x() > player_.position.x() ? 1 : -1;
  while (player_.position.x() != destination.x())
  {
    const geometry::Rectangle player_rect { player_.position + geometry::Position(step_x, 0), player_.size.x(), player_.size.y() };
    if (collides(player_rect))
    {
      player_.collide_x = true;
      break;
    }
    player_.position += geometry::Position(step_x, 0);
  }

  // Move on y axis
  const auto step_y = destination.y() > player_.position.y() ? 1 : -1;
  while (player_.position.y() != destination.y())
  {
    const geometry::Rectangle player_rect { player_.position + geometry::Position(0, step_y), player_.size };
    if (collides(player_rect))
    {
      player_.collide_y = true;
      break;
    }

    // If player is falling down we need to check for collision with platforms
    if (step_y == 1)
    {
      const auto platform_collide = [this, &player_rect]()
      {
        for (const auto& platform : level_.get_platforms())
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
        return false;
      }();
      if (platform_collide)
      {
        player_.collide_y = true;
        break;
      }
    }
    player_.position += geometry::Position(0, step_y);
  }

  /**
   * 4. Update player information based on collision
   */

  // Check if player hit something while walking
  if (player_.walking && player_.collide_x)
  {
    player_.walking = false;
  }

  // Check if player still jumping
  if (player_.jumping)
  {
    // Check if player hit something while jumping
    if (player_.collide_y)
    {
      if (player_.velocity.y() < 0)
      {
        // Player hit something while jumping up
        // Skip to "falling down" velocity
        player_.jump_tick = jump_velocity_fall_index;
      }
      else  // player_.velocity.y() > 0
      {
        // Player landed
        player_.jumping = false;
      }
    }
    else if (player_.jump_tick == jump_velocity.size() - 1u)
    {
      // Player jump ended
      player_.jumping = false;
    }
    else if (player_.jump_tick != 0 &&
             collides(geometry::Rectangle(player_.position + geometry::Position(0, 1),
                                          player_.size)))
    {
      // Player did not actually collide with the ground, but standing directly above it
      // and this isn't the first tick in the jump, so we can consider the jump to have
      // ended here
      player_.jumping = false;
    }
  }

  // Check if player is falling
  player_.falling = !player_.jumping &&
                    player_.velocity.y() > 0 &&
                    !player_.collide_y &&
                    !collides(geometry::Rectangle(player_.position + geometry::Position(0, 1),
                                                  player_.size));
}

bool Game::collides(const geometry::Rectangle& player_rect)
{
  for (const auto& aabb : level_.get_aabbs())
  {
    if (geometry::isColliding(player_rect, aabb))
    {
      return true;
    }
  }
  return false;
}
