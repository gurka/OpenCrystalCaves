#include "game_impl.h"

#include <cstdint>
#include <array>
#include <sstream>

#include "level_loader.h"
#include "misc.h"
#include "logger.h"
#include "object_manager.h"

static constexpr auto gravity = 8u;
static constexpr auto jump_velocity = misc::make_array<int>(0, -8, -8, -8, -4, -4, -2, -2, -2, -2, 2, 2, 2, 2, 4, 4);
static constexpr auto jump_velocity_fall_index = 10u;

static constexpr auto missile_size = geometry::Size(16, 16);
static constexpr auto missile_speed = misc::make_array(4, 4, 4, 4, 4, 4, 6, 8, 10, 12, 14);
static constexpr auto explosion_sprites = misc::make_array(28, 29, 30, 31, 30, 29, 28);

const Background Background::INVALID;
const Tile Tile::INVALID;
const Item Item::INVALID;

std::unique_ptr<Game> Game::create()
{
  return std::make_unique<GameImpl>();
}

bool GameImpl::init()
{
  if (!object_manager_.load("media/items.json"))
  {
    return false;
  }

  //level_ = LevelLoader::load_level(LevelId::MAIN_LEVEL);
  level_ = LevelLoader::load_level(LevelId::LEVEL_ONE);
  if (!level_)
  {
    return false;
  }

  player_.position = level_->player_spawn;

  score_ = 0u;
  num_ammo_ = 5u;
  num_lives_ = 3u;

  missile_.alive = false;

  return true;
}

void GameImpl::update(unsigned game_tick, const PlayerInput& player_input)
{
  (void)game_tick;  // Not needed atm

  // Clear objects_
  objects_.clear();

  // Update the level (e.g. moving platforms and other objects)
  update_level();

  // Update the player
  update_player(player_input);

  // Update items
  update_items();

  // Update missile
  update_missile();

  // Update enemies
  // ...

  // Update more?
  // ...
}

const Background& GameImpl::get_background() const
{
  if (level_->background_id != -1)
  {
    return object_manager_.get_background(level_->background_id);
  }
  else
  {
    return Background::INVALID;
  }
}

const Tile& GameImpl::get_tile(int tile_x, int tile_y) const
{
  if (tile_x < 0 || tile_x >= level_->width || tile_y < 0 || tile_y >= level_->height)
  {
    return Tile::INVALID;
  }

  const auto tile_id = level_->tile_ids[(tile_y * level_->width) + tile_x];
  if (tile_id != -1)
  {
    return object_manager_.get_tile(tile_id);
  }
  else
  {
    return Tile::INVALID;
  }
}

const Item& GameImpl::get_item(int tile_x, int tile_y) const
{
  if (tile_x < 0 || tile_x >= level_->width || tile_y < 0 || tile_y >= level_->height)
  {
    return Item::INVALID;
  }

  const auto item_id = level_->item_ids[(tile_y * level_->width) + tile_x];
  if (item_id != -1)
  {
    return object_manager_.get_item(item_id);
  }
  else
  {
    return Item::INVALID;
  }
}

std::string GameImpl::get_debug_info() const
{
  std::ostringstream oss;

  oss << "player position: (" << player_.position.x() << ", " << player_.position.y() << ")" << "\n";
  oss << "player velocity: (" << player_.velocity.x() << ", " << player_.velocity.y() << ")" << "\n";
  oss << "player collide: " << (player_.collide_x ? "x " : "_ ") << (player_.collide_y ? "y" : "_") << "\n";
  oss << "player walking: " << (player_.walking ? "true" : "false") << "\n";
  oss << "player jumping: " << (player_.jumping ? "true" : "false") << "\n";
  oss << "player falling: " << (player_.falling ? "true" : "false") << "\n";
  oss << "player shooting: " << (player_.shooting ? "true" : "false") << "\n";
  oss << "missile alive: " << (missile_.alive ? "true" : "false") << "\n";
  oss << "missile position: (" << missile_.position.x() << ", " << missile_.position.y() << ")" << "\n";

  return oss.str();
}

void GameImpl::update_level()
{
  // Update all MovingPlatforms
  for (auto& platform : level_->moving_platforms)
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
      if (!collides(new_player_pos, player_.size))
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

  // Add moving platforms to objects_
  for (auto& platform : level_->moving_platforms)
  {
    objects_.emplace_back(platform.position, platform.sprite_id, platform.num_sprites);
  }
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
      !collides(player_.position + geometry::Position(0, -1), player_.size))
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

    if (collides(new_player_pos, player_.size))
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
    if (collides(new_player_pos, player_.size) ||
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
             collides(player_.position + geometry::Position(0, 1), player_.size))
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
                    !collides(player_.position + geometry::Position(0, 1), player_.size);
}

void GameImpl::update_items()
{
  // Check if player hit an item
  // Player can cover at maximum 4 items
  // Check all 4 items, even though we might check the same item multiple times
  const std::array<geometry::Position, 4> positions =
  {
    geometry::Position((player_.position.x() + 0)                    / 16, (player_.position.y() + 0)                    / 16),
    geometry::Position((player_.position.x() + player_.size.x() - 1) / 16, (player_.position.y() + 0)                    / 16),
    geometry::Position((player_.position.x() + 0)                    / 16, (player_.position.y() + player_.size.y() - 1) / 16),
    geometry::Position((player_.position.x() + player_.size.x() - 1) / 16, (player_.position.y() + player_.size.y() - 1) / 16)
  };

  for (const auto& position : positions)
  {
    const auto& item = get_item(position.x(), position.y());
    if (item.valid())
    {
      if (item.get_type() == 0)
      {
        LOG_DEBUG("Player took item of type crystal (%d)", item.get_type());
        score_ += 500;  // TODO: Check how much score a crystal gives
      }
      else if (item.get_type() == 1)
      {
        LOG_DEBUG("Player took item of type ammo (%d), amount: %d", item.get_type(), item.get_amount());
        num_ammo_ += item.get_amount();

        // 99 is max ammo
        num_ammo_ = num_ammo_ > 99 ? 99 : num_ammo_;
      }
      else if (item.get_type() == 2)
      {
        LOG_DEBUG("Player took item of type score (%d), amount: %d", item.get_type(), item.get_amount());
        score_ += item.get_amount();
      }

      remove_item(position.x(), position.y());
    }
  }
}

void GameImpl::update_missile()
{
  // Check current explosion (if alive)
  if (explosion_.alive)
  {
    explosion_.frame += 1;

    if (explosion_.frame >= explosion_sprites.size())
    {
      explosion_.alive = false;
    }
  }

  // Check current missile (if alive)
  if (missile_.alive)
  {
    // Move it and increase frame number
    // TODO: We might need to move it 1 pixel at a time and check collision at each pixel
    //       so that the missile doesn't go through an enemy
    const auto speed = missile_.frame < missile_speed.size() ? missile_speed[missile_.frame] : missile_speed.back();
    missile_.position += geometry::Position((missile_.right ? speed : -speed), 0);
    missile_.frame += 1;

    // Check collision with enemy or wall, after moving it
    // TODO: This doesn't work 100% since player size is smaller than missile size
    //       Need to write a collidies() that takes position and size
    if (collides(missile_.position, missile_size))
    {
      missile_.alive = false;

      explosion_.alive = true;
      explosion_.frame = 0;
      explosion_.position = missile_.position;
    }
    else if (missile_.frame > 30)  // TODO: verify when exactly the missile disappears
    {
      missile_.alive = false;
    }
  }
  else if (player_.shooting)
  {
    // Player wants to shoot new missile
    if (num_ammo_ > 0)
    {
      missile_.alive = true;
      missile_.frame = 0;
      if (player_.direction == Player::Direction::right)
      {
        missile_.right = true;
        missile_.position = player_.position + geometry::Position(player_.size.x(), 0);
      }
      else
      {
        missile_.right = false;
        missile_.position = player_.position - geometry::Position(player_.size.x(), 0);
      }

      num_ammo_ -= 1;
    }
    else
    {
      // Play "no ammo" sound when we have audio
    }
  }

  // Add explosion to objects_ if alive
  if (explosion_.alive)
  {
    objects_.emplace_back(explosion_.position, explosion_sprites[explosion_.frame], 1);
  }

  // Add missile to objects_ if alive
  if (missile_.alive)
  {
    objects_.emplace_back(missile_.position, (missile_.right ? 296 : 302), 6);
  }
}

bool GameImpl::collides(const geometry::Position& position, const geometry::Size& size)
{
  // Note: this function only works with size x and y <= 16
  // With size 16x16 the object can cover at maximum 4 tiles
  // Check all 4 tiles, even though we might check the same tile multiple times
  const std::array<geometry::Position, 4> positions =
  {
    geometry::Position((position.x() + 0)            / 16, (position.y() + 0)            / 16),
    geometry::Position((position.x() + size.x() - 1) / 16, (position.y() + 0)            / 16),
    geometry::Position((position.x() + 0)            / 16, (position.y() + size.y() - 1) / 16),
    geometry::Position((position.x() + size.x() - 1) / 16, (position.y() + size.y() - 1) / 16)
  };
  for (const auto& position : positions)
  {
    if (get_tile(position.x(), position.y()).is_solid())
    {
      return true;
    }
  }
  return false;
}

bool GameImpl::player_on_platform(const geometry::Position& player_position)
{
  // Need to check both static platforms (e.g. foreground items with SOLID_TOP)
  // and moving platforms

  // Standing on a static platform requires the player to stand on the edge of a tile
  if ((player_position.y() + player_.size.y() - 1) % 16 == 0)
  {
    // Player can be on either 1 or 2 tiles, check both (or same...)
    if (get_tile( player_position.x() / 16,                     (player_position.y() + player_.size.y() - 1) / 16).is_solid_top() ||
        get_tile((player_position.x() + player_.size.x()) / 16, (player_position.y() + player_.size.y() - 1) / 16).is_solid_top())
    {
      return true;
    }
  }

  // Check moving platforms
  for (const auto& platform : level_->moving_platforms)
  {
    // Player only collides if standing exactly on top of the platform, just like with static platforms
    if ((player_position.y() + player_.size.y() - 1 == platform.position.y()) &&
        (player_position.x() < platform.position.x() + 16) &&
        (player_position.x() + player_.size.x() > platform.position.x()))
    {
      return true;
    }
  }

  return false;
}

void GameImpl::remove_item(int tile_x, int tile_y)
{
  level_->item_ids[(tile_y * level_->width) + tile_x] = -1;  // TODO: Item::INVALID_ID (ObjectManager::INVALID_ID?)
}
