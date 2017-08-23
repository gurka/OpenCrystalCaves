#include "game.h"

#include <sstream>
#include <string>

#include "draw.h"
#include "item_loader.h"
#include "level_loader.h"

namespace
{

template <typename... T>
constexpr auto make_array(T&&... values) -> std::array<typename std::decay<typename std::common_type<T...>::type>::type, sizeof...(T)>
{
  return std::array<typename std::decay<typename std::common_type<T...>::type>::type, sizeof...(T)>{std::forward<T>(values)...};
}

}

constexpr std::array<int, 12> Game::Player::sprite_walking_right;
constexpr std::array<int, 12> Game::Player::sprite_walking_left;

Game::Game()
  : player_(),
    initialized_(false),
    surface_game_(nullptr, SDL_FreeSurface),
    sprite_manager_(),
    items_(),
    level_(),
    update_tick_(0u),
    draw_debug_(false),
    draw_aabbs_(false),
    collide_x_(false),
    collide_y_(false),
    input_left_(false),
    input_right_(false),
    input_jump_(false)
{
}

bool Game::init(const SDL_Surface* windowSurface)
{
  /////////////////////////////////////////////////////////////////////////////
  //
  // Create layer surfaces
  //
  /////////////////////////////////////////////////////////////////////////////
  const auto* windowSurfaceFormat = windowSurface->format;

  surface_game_.reset(SDL_CreateRGBSurface(0,
                                           CAMERA_WIDTH,
                                           CAMERA_HEIGHT,
                                           windowSurfaceFormat->BitsPerPixel,
                                           windowSurfaceFormat->Rmask,
                                           windowSurfaceFormat->Gmask,
                                           windowSurfaceFormat->Bmask,
                                           windowSurfaceFormat->Amask));
  if (!surface_game_)
  {
    return false;
  }
  SDL_SetColorKey(surface_game_.get(), SDL_TRUE, SDL_MapRGB(windowSurface->format, 0, 180, 0));

  /////////////////////////////////////////////////////////////////////////////
  //
  // Load sprites, items and levels
  //
  /////////////////////////////////////////////////////////////////////////////
  if (!sprite_manager_.load_tileset("media/sprites.bmp"))
  {
    return false;
  }

  items_ = ItemLoader::load_items("media/items.json");
  if (items_.empty())
  {
    return false;
  }

  level_ = LevelLoader::load_level("media/level1.json");
  if (!level_.valid())
  {
    return false;
  }

  /////////////////////////////////////////////////////////////////////////////
  //
  // Init player
  //
  /////////////////////////////////////////////////////////////////////////////
  player_.position = Vector<int>(16, 16);
  player_.direction = Player::Direction::right;
  player_.state = Player::State::still;


  initialized_ = true;
  return true;
}

void Game::update(const Input& input)
{
  static constexpr std::array<int, 16> jump_velocity   = { -8, -8, -8, -4, -4, -2, -2, -2, -2, 0, 2, 2, 2, 2, 4, 4 };
  static constexpr std::size_t jump_velocity_fall_index = 10;

  update_tick_++;

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
      if (input.left.pressed)
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
      else if (input.right.pressed)
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
      else
      {
        // Set zero x velocity
        player_.velocity = Vector<int>(0, player_.velocity.getY());
      }
      break;
    }
  }


  ///////////////////////////////////////////////////////////////////
  //
  //  Move player towards destination and check for collision
  //
  ///////////////////////////////////////////////////////////////////
  auto collide_on_x = false;
  auto collide_on_y = false;

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
      collide_on_x = true;
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
      collide_on_y = true;
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
      if (collide_on_y && player_.velocity.getY() > 0)
      {
        // Player is falling down but hit something (the ground), set new state
        player_.state = player_.velocity.getX() == 0 ? Player::State::still : Player::State::walking;
      }
      else if (collide_on_y)
      {
        // Player hit something while jumping, set jump_tick so that he starts falling
        player_.jump_tick = jump_velocity_fall_index;
      }
      break;
    }

    case Player::State::still:
    {
      if (player_.velocity.getX() != 0 && !collide_on_x)
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
      if (player_.velocity.getX() == 0 || collide_on_x)
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
  if (input.space.pressed && collide_on_y && player_.state != Player::State::jumping)
  {
    // Player wants to jump, change state and set start jump velocity
    player_.state = Player::State::jumping;
    player_.jump_tick = 0;

    // TODO: Current implementation will cause the jump sprite to be set one tick
    // if the player tries to jump just under a blocking tile
  }


  ///////////////////////////////////////////////////////////////////
  //
  //  Debug
  //
  ///////////////////////////////////////////////////////////////////
  if (input.num_1.pressed && !input.num_1.repeated)
  {
    draw_aabbs_ = !draw_aabbs_;
  }
  if (input.num_2.pressed && !input.num_2.repeated)
  {
    draw_debug_ = !draw_debug_;
  }
  collide_x_ = collide_on_x;
  collide_y_ = collide_on_y;
  input_left_ = input.left.pressed;
  input_right_ = input.right.pressed;
  input_jump_ = input.space.pressed;
}

void Game::render(SDL_Surface* surface_screen) const
{
  // FROM CRYSTAL CAVES:
  // Visible blocks on x axis: 20  (20 tiles * 16 pixels = 320 pixels)
  // Visible blocks on y axis: 12  (12 tiles * 16 pixels = 192 pixels)
  // Ratio seems to be 1.1875 (stretched on y axis)

  // Calculate where the camera is
  const geometry::Position camera_pos(player_.position +  // player position
                                      geometry::Position(8, 8) -  // plus half player size
                                      geometry::Position(CAMERA_WIDTH / 2, CAMERA_HEIGHT / 2));  // minus half camera size

  // Create a Rectangle that represents the camera and adjust the position so that nothing outside the level is visible
  const geometry::Rectangle camera(math::clamp(camera_pos.getX(),
                                               0,
                                               (level_.get_tile_width() * 16) - CAMERA_WIDTH),
                                   math::clamp(camera_pos.getY(),
                                               0,
                                               (level_.get_tile_height() * 16) - CAMERA_HEIGHT),
                                   CAMERA_WIDTH,
                                   CAMERA_HEIGHT);

  // Clear game surface (background now)
  SDL_FillRect(surface_game_.get(), nullptr, SDL_MapRGB(surface_game_->format, 33, 33, 33));

  // This is the visible part of the level
  const auto start_tile_x = camera.position.getX() > 0 ? camera.position.getX() / 16 : 0;
  const auto start_tile_y = camera.position.getY() > 0 ? camera.position.getY() / 16 : 0;
  const auto end_tile_x = (camera.position.getX() + camera.size.getX()) / 16;
  const auto end_tile_y = (camera.position.getY() + camera.size.getY()) / 16;

  // SpriteSheet surface
  auto surface = sprite_manager_.get_surface();

  // Render all background tiles
  {
    for (int tile_y = start_tile_y; tile_y <= end_tile_y; tile_y++)
    {
      for (int tile_x = start_tile_x; tile_x <= end_tile_x; tile_x++)
      {
        auto item_id = level_.get_tile_background(tile_x, tile_y);
        if (item_id != Item::invalid)
        {
          const auto& item = items_[item_id];
          const auto sprite_id = [&item, &tile_x, &tile_y]()
          {
            if (item.is_multiple_2x2())
            {
              return item.get_sprite() + ((tile_y % 2) * 4) + (tile_x % 2);
            }
            else if (item.is_multiple_4x2())
            {
              return item.get_sprite() + ((tile_y % 2) * 4) + (tile_x % 4);
            }
            else
            {
              return item.get_sprite();
            }
          }();
          auto src_rect = sprite_manager_.get_rect_for_tile(sprite_id);
          SDL_Rect dest_rect
          {
            (tile_x * 16) - camera.position.getX(),
            (tile_y * 16) - camera.position.getY(),
            16,
            16
          };
          SDL_BlitSurface(surface, &src_rect, surface_game_.get(), &dest_rect);
        }
      }
    }
  }

  // Render all middleground tiles
  {
    for (int tile_y = start_tile_y; tile_y <= end_tile_y; tile_y++)
    {
      for (int tile_x = start_tile_x; tile_x <= end_tile_x; tile_x++)
      {
        auto item_id = level_.get_tile_middleground(tile_x, tile_y);
        if (item_id != Item::invalid)
        {
          const auto& item = items_[item_id];
          const auto sprite_id = item.get_sprite();
          auto src_rect = sprite_manager_.get_rect_for_tile(sprite_id);
          SDL_Rect dest_rect
          {
            (tile_x * 16) - camera.position.getX(),
            (tile_y * 16) - camera.position.getY(),
            16,
            16
          };
          SDL_BlitSurface(surface, &src_rect, surface_game_.get(), &dest_rect);
        }
      }
    }
  }

  // Render player
  {
    SDL_Rect src_rect = [this]()
    {
      if (player_.direction == Player::Direction::right)
      {
        if (player_.state == Player::State::still)
        {
          return sprite_manager_.get_rect_for_tile(player_.sprite_standing_right);
        }
        else if (player_.state == Player::State::walking)
        {
          return sprite_manager_.get_rect_for_tile(player_.sprite_walking_right[player_.animation_tick % player_.sprite_walking_right.size()]);
        }
        else  // player_.state == Player::State::jumping
        {
          return sprite_manager_.get_rect_for_tile(player_.sprite_jumping_right);
        }
      }
      else  // player_.direction == Player::Direction::left
      {
        if (player_.state == Player::State::still)
        {
          return sprite_manager_.get_rect_for_tile(player_.sprite_standing_left);
        }
        else if (player_.state == Player::State::walking)
        {
          return sprite_manager_.get_rect_for_tile(player_.sprite_walking_left[player_.animation_tick % player_.sprite_walking_left.size()]);
        }
        else  // player_.state == Player::State::jumping
        {
          return sprite_manager_.get_rect_for_tile(player_.sprite_jumping_left);
        }
      }
    }();
    auto player_render_pos = player_.position - camera.position;
    SDL_Rect dest_rect { player_render_pos.getX(), player_render_pos.getY(), 16, 16 };
    SDL_BlitSurface(surface, &src_rect, surface_game_.get(), &dest_rect);
  }

  // Render all foreground items visible in the level
  {
    for (int tile_y = start_tile_y; tile_y <= end_tile_y; tile_y++)
    {
      for (int tile_x = start_tile_x; tile_x <= end_tile_x; tile_x++)
      {
        auto item_id = level_.get_tile_foreground(tile_x, tile_y);
        if (item_id != Item::invalid)
        {
          const auto& item = items_[item_id];
          const auto sprite_id = [&item](unsigned update_tick)
          {
            if (item.is_animated())
            {
              return item.get_sprite() + static_cast<int>(update_tick % item.get_sprite_count());
            }
            else
            {
              return item.get_sprite();
            }
          }(update_tick_);
          auto src_rect = sprite_manager_.get_rect_for_tile(sprite_id);
          SDL_Rect dest_rect
          {
            (tile_x * 16) - camera.position.getX(),
            (tile_y * 16) - camera.position.getY(),
            16,
            16
          };
          SDL_BlitSurface(surface, &src_rect, surface_game_.get(), &dest_rect);
        }
      }
    }
  }

  // Debug
  {
    if (draw_aabbs_)
    {
      // Draw aabbs
      for (const auto& aabb : level_.get_aabbs())
      {
        if (geometry::isColliding(camera, aabb))
        {
          // Adjust the aabb position based on camera and render it
          draw::rectangle(geometry::Rectangle(aabb.position - camera.position, aabb.size),
                          { 255u, 0u, 0u, 0u },
                          surface_game_.get());
        }
      }
    }
  }

  // Render game surface to screen surface (scaled)
  SDL_Rect src_rect = { 0, 0, CAMERA_WIDTH, CAMERA_HEIGHT };
  SDL_Rect dest_rect = { 0, 0, CAMERA_WIDTH * SCREEN_SCALE, CAMERA_HEIGHT * SCREEN_SCALE };
  SDL_BlitScaled(surface_game_.get(), &src_rect, surface_screen, &dest_rect);

  if (draw_debug_)
  {
    // Debug info
    const auto camera_str  = "camera pixel: (" + std::to_string(camera.position.getX()) + ", " + std::to_string(camera.position.getY()) + ")";
    const auto pixel_str   = "player pixel: (" + std::to_string(player_.position.getX()) + ", " + std::to_string(player_.position.getY()) + ")";
    const auto tiles_str   = "player tiles: (" + std::to_string(player_.position.getX() / 16) + ", " + std::to_string(player_.position.getY() / 16) + ")";
    const auto collide_str = std::string("collide: ") + (collide_x_ ? "x " : "_ ") + (collide_y_ ? "y" : "_");
    const auto input_str   = std::string("input: ") + (input_left_ ? "left " : "____ ") + (input_right_ ? "right " : "_____ ") + (input_jump_ ? "jump" : "____");

    draw::text(5,  25, camera_str,  { 255u, 0u, 0u, 0u}, surface_screen);
    draw::text(5,  45, pixel_str,   { 255u, 0u, 0u, 0u}, surface_screen);
    draw::text(5,  65, tiles_str,   { 255u, 0u, 0u, 0u}, surface_screen);
    draw::text(5,  85, collide_str, { 255u, 0u, 0u, 0u}, surface_screen);
    draw::text(5, 105, input_str,   { 255u, 0u, 0u, 0u}, surface_screen);

  }
}
