#include "game.h"

#include <cstdint>
#include <array>

#include "item_loader.h"
#include "level_loader.h"

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

  player_.position = Vector<int>(16, 16);
  player_.direction = Player::Direction::right;
  player_.state = Player::State::still;

  return true;
}

void Game::update(const PlayerInput& player_input)
{
  static constexpr std::array<int, 16> jump_velocity   = { -8, -8, -8, -4, -4, -2, -2, -2, -2, 0, 2, 2, 2, 2, 4, 4 };
  static constexpr std::size_t jump_velocity_fall_index = 10;

  ///////////////////////////////////////////////////////////////////
  //
  //  Calculate player velocity based on input and player state
  //
  ///////////////////////////////////////////////////////////////////

  // Set y velocity
  switch (player_.state)
  {
    case Player::State::still:
    case Player::State::walking:
    {
      // Just apply gravity
      player_.velocity = Vector<int>(player_.velocity.getX(), 8);
      break;
    }

    case Player::State::jumping:
    {
      if (player_.jump_tick < jump_velocity.size())
      {
        player_.velocity = Vector<int>(player_.velocity.getX(), jump_velocity[player_.jump_tick]);
      }
      else
      {
        // Just apply gravity
        player_.velocity = Vector<int>(player_.velocity.getX(), 8);
      }
      player_.jump_tick += 1;
      break;
    }
  }

  // Set x velocity
  switch (player_.state)
  {
    case Player::State::still:
    case Player::State::walking:
    case Player::State::jumping:
    {
      if ((player_input.left && player_input.right) ||
          (!player_input.left && !player_input.right))
      {
        // Set zero x velocity
        player_.velocity = Vector<int>(0, player_.velocity.getY());
      }
      else if (player_input.left)
      {
        // First step is 2 pixels / tick, then 4 pixels / tick
        if (player_.velocity.getX() == -2)
        {
          player_.velocity = Vector<int>(-4, player_.velocity.getY());
        }
        else
        {
          player_.velocity = Vector<int>(-2, player_.velocity.getY());
        }
      }
      else if (player_input.right)
      {
        // Same as above
        if (player_.velocity.getX() == 2)
        {
          player_.velocity = Vector<int>(4, player_.velocity.getY());
        }
        else
        {
          player_.velocity = Vector<int>(2, player_.velocity.getY());
        }
      }
      break;
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

  const auto step_x = destination.getX() > player_.position.getX() ? 1 : -1;
  while (player_.position.getX() != destination.getX())
  {
    const geometry::Rectangle player_rect { player_.position + geometry::Position(step_x, 0), 16, 16 };
    if (collides(player_rect))
    {
      player_collide_x_ = true;
      break;
    }
    player_.position += geometry::Position(step_x, 0);
  }

  const auto step_y = destination.getY() > player_.position.getY() ? 1 : -1;
  while (player_.position.getY() != destination.getY())
  {
    const geometry::Rectangle player_rect { player_.position + geometry::Position(0, step_y), 16, 16 };
    if (collides(player_rect))
    {
      player_collide_y_ = true;
      break;
    }
    player_.position += geometry::Position(0, step_y);
  }


  ///////////////////////////////////////////////////////////////////
  //
  //  Set player information
  //
  ///////////////////////////////////////////////////////////////////
  switch (player_.state)
  {
    case Player::State::jumping:
    {
      if (player_collide_y_ && player_.velocity.getY() > 0)
      {
        // Player is falling down but hit something (the ground), set new state
        player_.state = player_.velocity.getX() == 0 ? Player::State::still : Player::State::walking;
      }
      else if (player_collide_y_)
      {
        // Player hit something while jumping, set jump_tick so that he starts falling
        player_.jump_tick = jump_velocity_fall_index;
      }
      break;
    }

    case Player::State::still:
    {
      if (player_.velocity.getX() != 0 && !player_collide_x_)
      {
        // Player started to move, change to state walking
        player_.state = Player::State::walking;

        // And reset animation_start_tick
        player_.animation_tick = 0;
      }
      break;
    }

    case Player::State::walking:
    {
      if (player_.velocity.getX() == 0 || player_collide_x_)
      {
        // Player was walking and stopped or collided with something
        player_.state = Player::State::still;
      }
      else
      {
        player_.animation_tick += 1;
      }
      break;
    }
  }

  // Set player direction
  if (player_.velocity.getX() < 0)
  {
    player_.direction = Player::Direction::left;
  }
  else if (player_.velocity.getX() > 0)
  {
    player_.direction = Player::Direction::right;
  }

  // Check player jump
  if (player_input.jump && player_collide_y_ && player_.state != Player::State::jumping)
  {
    // Player wants to jump, change state and set start jump velocity
    player_.state = Player::State::jumping;
    player_.jump_tick = 0;

    // TODO: Current implementation will cause the jump sprite to be set one tick
    // if the player tries to jump just under a blocking tile
  }
}
