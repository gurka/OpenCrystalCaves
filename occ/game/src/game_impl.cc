#include "game_impl.h"

#include <cstdint>
#include <array>

#include "item_loader.h"
#include "level_loader.h"
#include "misc.h"
#include "logger.h"

static constexpr auto gravity = 8u;
static constexpr auto jump_velocity = misc::make_array<int>(0, -8, -8, -8, -4, -4, -2, -2, -2, -2, 2, 2, 2, 2, 4, 4);
static constexpr auto jump_velocity_fall_index = 10u;

std::unique_ptr<Game> Game::create()
{
  return std::make_unique<GameImpl>();
}

bool GameImpl::init()
{
  items_ = ItemLoader::load_items("media/items.json");
  if (items_.empty())
  {
    return false;
  }

  level_ = LevelLoader::load_level(LevelId::MAIN_LEVEL);
  //level_ = LevelLoader::load_level(LevelId::LEVEL_ONE);
  if (!level_)
  {
    return false;
  }

  player_.position = level_->get_player_spawn();

  return true;
}

void GameImpl::update(unsigned game_tick, const PlayerInput& player_input)
{
  // Update the level (e.g. moving platforms and other objects)
  update_level(game_tick);

  // Update the player
  update_player(player_input);

  // Update enemies
  // ...

  // Update more?
  // ...
}

void GameImpl::update_level(unsigned game_tick)
{
  // Update all MovingPlatforms
  for (auto& platform : level_->get_moving_platforms())
  {
    // Update platform
    const auto platform_velocity = platform.forward ? platform.velocity_forward : -platform.velocity_forward;
    const auto player_on_platform = (player_.position.y() + player_.size.y() == platform.position.y()) &&
                                    (player_.position.x() < platform.position.x() + 16) &&
                                    (player_.position.x() + player_.size.x() > platform.position.x());

    // Move player if standing on platform
    if (player_on_platform)
    {
      // Only move player if not colliding with any static objects
      const auto new_player_pos = player_.position + platform_velocity;
      if (!player_collides(new_player_pos))
      {
        player_.position = new_player_pos;
      }
    }

    // Move platform
    platform.position += platform_velocity;

    // Check if platform reached end / start
    if ((platform.forward && platform.position == platform.position_end) ||
        (!platform.forward && platform.position == platform.position_start))
    {
      // Change direction
      platform.forward = !platform.forward;
    }
  }

  // Update additional objects in the level
  level_->update(game_tick);
}

void GameImpl::update_player(const PlayerInput& player_input)
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
      !player_collides(player_.position + geometry::Position(0, -1)))
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
    const auto new_player_pos = player_.position + geometry::Position(step_x, 0);

    if (player_collides(new_player_pos))
    {
      player_.collide_x = true;
      break;
    }

    player_.position = new_player_pos;
  }

  // Move on y axis
  const auto step_y = destination.y() > player_.position.y() ? 1 : -1;
  while (player_.position.y() != destination.y())
  {
    const auto new_player_pos = player_.position + geometry::Position(0, step_y);

    // If player is falling down (step_y == 1) we need to check for collision with platforms
    if (player_collides(new_player_pos) ||
        (step_y == 1 && player_on_platform(new_player_pos)))
    {
      player_.collide_y = true;
      break;
    }

    player_.position = new_player_pos;
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
             player_collides(player_.position + geometry::Position(0, 1)))
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
                    !player_collides(player_.position + geometry::Position(0, 1));
}

bool GameImpl::player_collides(const geometry::Position& position)
{
  // Player can cover at maximum 4 tiles
  // Check all 4 tiles, even though we might check the same tile multiple times
  return items_[level_->get_tile_foreground( position.x() / 16,                          position.y() / 16)                        ].is_solid() ||
         items_[level_->get_tile_foreground((position.x() + player_.size.x() - 1) / 16,  position.y() / 16)                        ].is_solid() ||
         items_[level_->get_tile_foreground( position.x() / 16,                         (position.y() + player_.size.y() - 1) / 16)].is_solid() ||
         items_[level_->get_tile_foreground((position.x() + player_.size.x() - 1) / 16, (position.y() + player_.size.y() - 1) / 16)].is_solid();
}

bool GameImpl::player_on_platform(const geometry::Position& position)
{
  // Need to check both static platforms (e.g. foreground items with SOLID_TOP)
  // and moving platforms

  // Standing on a static platform requires the player to stand on the edge of a tile
  if ((position.y() + player_.size.y() - 1) % 16 == 0)
  {
    // Player can be on either 1 or 2 tiles, check both (or same...)
    if (items_[level_->get_tile_foreground( position.x() / 16,                     (position.y() + player_.size.y() - 1) / 16)].is_solid_top() ||
        items_[level_->get_tile_foreground((position.x() + player_.size.x()) / 16, (position.y() + player_.size.y() - 1) / 16)].is_solid_top())
    {
      return true;
    }
  }

  // Check moving platforms
  for (const auto& platform : level_->get_moving_platforms())
  {
    // Player only collides if standing exactly on top of the platform, just like with static platforms
    if ((position.y() + player_.size.y() - 1 == platform.position.y()) &&
        (position.x() < platform.position.x() + 16) &&
        (position.x() + player_.size.x() > platform.position.x()))
    {
      return true;
    }
  }

  return false;
}
