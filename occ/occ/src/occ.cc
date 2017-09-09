#include <cassert>
#include <cstdio>
#include <cstdlib>

#include <memory>

#include <SDL.h>

#include "input.h"
#include "spritemgr.h"
#include "logger.h"
#include "draw.h"
#include "game.h"
#include "level.h"
#include "player.h"
#include "item.h"
#include "object.h"
#include "player_input.h"
#include "sdl_wrapper.h"
#include "math.h"

using Camera = geometry::Rectangle;

// The size of the game camera
// TODO: It seems as if Crystal Caves actually renders 12½ tiles on y axis - verify this and adjust accordingly
static constexpr geometry::Size CAMERA_SIZE = geometry::Size(320, 192);

// The size of the game camera after stretching, which is done in the original Crystal Caves
static constexpr geometry::Size CAMERA_SIZE_STRETCHED = geometry::Size(CAMERA_SIZE.x(), CAMERA_SIZE.y() * 1.2f);

// The size of the game camera after scaling - this should be based on the (dynamic) window size in the future
static constexpr geometry::Size CAMERA_SIZE_SCALED = CAMERA_SIZE_STRETCHED * 3.0f;

// The size of the game window
static constexpr geometry::Size WINDOW_SIZE = CAMERA_SIZE_SCALED;

// Game variables
static std::unique_ptr<Game> game;
static SpriteManager sprite_manager;
static bool paused(false);

// Used by the render_x functions and should be up to date before each call to render_x()
static std::unique_ptr<Surface> game_surface;
static Camera game_camera;
static unsigned game_tick;

// Debug information
static bool debug(false);

void read_input(Input* input)
{
  assert(input != nullptr);

  // Set any input that is pressed as repeated here
  input->up.repeated    = input->up.pressed;
  input->down.repeated  = input->down.pressed;
  input->left.repeated  = input->left.pressed;
  input->right.repeated = input->right.pressed;
  input->z.repeated     = input->z.pressed;
  input->x.repeated     = input->x.pressed;
  input->num_1.repeated = input->num_1.pressed;
  input->num_2.repeated = input->num_2.pressed;
  input->enter.repeated = input->enter.pressed;
  input->space.repeated = input->space.pressed;

  // Read events
  SDL_Event event;
  while (SDL_PollEvent(&event) != 0)
  {
    if (event.type == SDL_QUIT)
    {
      input->quit = true;
      return;  // Quit asap
    }

    else if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP)
    {
      switch (event.key.keysym.sym)
      {
        case SDLK_UP:
          input->up.pressed = event.type == SDL_KEYDOWN;
          if (!input->up.pressed) input->up.repeated = false;
          break;

        case SDLK_DOWN:
          input->down.pressed = event.type == SDL_KEYDOWN;
          if (!input->down.pressed) input->down.repeated = false;
          break;

        case SDLK_LEFT:
          input->left.pressed = event.type == SDL_KEYDOWN;
          if (!input->left.pressed) input->left.repeated = false;
          break;

        case SDLK_RIGHT:
          input->right.pressed = event.type == SDL_KEYDOWN;
          if (!input->right.pressed) input->right.repeated = false;
          break;

        case SDLK_z:
          input->z.pressed = event.type == SDL_KEYDOWN;
          if (!input->z.pressed) input->z.repeated = false;
          break;

        case SDLK_x:
          input->x.pressed = event.type == SDL_KEYDOWN;
          if (!input->x.pressed) input->x.repeated = false;
          break;

        case SDLK_1:
          input->num_1.pressed = event.type == SDL_KEYDOWN;
          if (!input->num_1.pressed) input->num_1.repeated = false;
          break;

        case SDLK_2:
          input->num_2.pressed = event.type == SDL_KEYDOWN;
          if (!input->num_2.pressed) input->num_2.repeated = false;
          break;

        case SDLK_RETURN:
          input->enter.pressed = event.type == SDL_KEYDOWN;
          if (!input->enter.pressed) input->enter.repeated = false;
          break;

        case SDLK_SPACE:
          input->space.pressed = event.type == SDL_KEYDOWN;
          if (!input->space.pressed) input->space.repeated = false;
          break;

        case SDLK_ESCAPE:
          input->quit = true;
          return;

        default:
          break;
      }
    }
  }
}

void render_background()
{
  const auto& level = game->get_level();
  const auto& items = game->get_items();

  const auto start_tile_x = game_camera.position.x() > 0 ? game_camera.position.x() / 16 : 0;
  const auto start_tile_y = game_camera.position.y() > 0 ? game_camera.position.y() / 16 : 0;
  const auto end_tile_x = (game_camera.position.x() + game_camera.size.x()) / 16;
  const auto end_tile_y = (game_camera.position.y() + game_camera.size.y()) / 16;

  for (int tile_y = start_tile_y; tile_y <= end_tile_y; tile_y++)
  {
    for (int tile_x = start_tile_x; tile_x <= end_tile_x; tile_x++)
    {
      auto item_id = level.get_tile_background(tile_x, tile_y);
      if (item_id != Item::invalid)
      {
        const auto& item = items[item_id];
        const auto sprite_id = [&item, &tile_x, &tile_y]()
        {
          if (item.is_multiple_2x2())
          {
            return item.get_sprite() + (((tile_y + 1) % 2) * 4) + (tile_x % 2);
          }
          else if (item.is_multiple_4x2())
          {
            return item.get_sprite() + (((tile_y + 1) % 2) * 4) + (tile_x % 4);
          }
          else
          {
            return item.get_sprite();
          }
        }();
        const auto src_rect = sprite_manager.get_rect_for_tile(sprite_id);
        const geometry::Rectangle dest_rect
        {
          (tile_x * 16) - game_camera.position.x(),
          (tile_y * 16) - game_camera.position.y(),
          16,
          16
        };
        game_surface->blit_surface(sprite_manager.get_surface(), src_rect, dest_rect, BlitType::CROP);
      }
    }
  }
}

void render_player()
{
  // Player sprite ids
  static constexpr int sprite_standing_right = 260;
  static constexpr std::array<int, 12> sprite_walking_right =
  {
      260, 261, 262, 263, 264, 265, 266, 267, 268, 269, 270, 271
  };
  static constexpr int sprite_jumping_right = 284;
  static constexpr int sprite_shooting_right = 286;

  static constexpr int sprite_standing_left = 272;
  static constexpr std::array<int, 12> sprite_walking_left =
  {
      272, 273, 274, 275, 276, 277, 278, 279, 280, 281, 282, 283
  };
  static constexpr int sprite_jumping_left = 285;
  static constexpr int sprite_shooting_left = 287;

  const auto& player = game->get_player();

  const geometry::Rectangle src_rect = [&player]()
  {
    // Sprite selection priority: (currently 'shooting' means pressing shoot button without ammo)
    // If walking:
    //   1. Jumping or falling
    //   2. Walking
    // Else:
    //   1. Shooting
    //   2. Jumping or falling
    //   3. Standing still

    if (player.direction == Player::Direction::right)
    {
      if (player.walking)
      {
        if (player.jumping || player.falling)
        {
          return sprite_manager.get_rect_for_tile(sprite_jumping_right);
        }
        else
        {
          return sprite_manager.get_rect_for_tile(sprite_walking_right[player.walk_tick % sprite_walking_right.size()]);
        }
      }
      else
      {
        if (player.shooting)
        {
          return sprite_manager.get_rect_for_tile(sprite_shooting_right);
        }
        else if (player.jumping || player.falling)
        {
          return sprite_manager.get_rect_for_tile(sprite_jumping_right);
        }
        else
        {
          return sprite_manager.get_rect_for_tile(sprite_standing_right);
        }
      }
    }
    else  // player_.direction == Player::Direction::left
    {
      if (player.walking)
      {
        if (player.jumping || player.falling)
        {
          return sprite_manager.get_rect_for_tile(sprite_jumping_left);
        }
        else
        {
          return sprite_manager.get_rect_for_tile(sprite_walking_left[player.walk_tick % sprite_walking_left.size()]);
        }
      }
      else
      {
        if (player.shooting)
        {
          return sprite_manager.get_rect_for_tile(sprite_shooting_left);
        }
        else if (player.jumping || player.falling)
        {
          return sprite_manager.get_rect_for_tile(sprite_jumping_left);
        }
        else
        {
          return sprite_manager.get_rect_for_tile(sprite_standing_left);
        }
      }
    }
  }();
  const auto player_render_pos = player.position - game_camera.position;

  // Note: player size is 12x16 but the sprite is 16x16 so we need to adjust where
  // the player is rendered
  const geometry::Rectangle dest_rect { player_render_pos.x() - 2, player_render_pos.y(), 16, 16 };

  game_surface->blit_surface(sprite_manager.get_surface(), src_rect, dest_rect, BlitType::CROP);
}

void render_foreground(bool in_front)
{
  const auto& level = game->get_level();
  const auto& items = game->get_items();

  const auto start_tile_x = game_camera.position.x() > 0 ? game_camera.position.x() / 16 : 0;
  const auto start_tile_y = game_camera.position.y() > 0 ? game_camera.position.y() / 16 : 0;
  const auto end_tile_x = (game_camera.position.x() + game_camera.size.x()) / 16;
  const auto end_tile_y = (game_camera.position.y() + game_camera.size.y()) / 16;

  for (int tile_y = start_tile_y; tile_y <= end_tile_y; tile_y++)
  {
    for (int tile_x = start_tile_x; tile_x <= end_tile_x; tile_x++)
    {
      auto item_id = level.get_tile_foreground(tile_x, tile_y);
      if (item_id != Item::invalid)
      {
        const auto& item = items[item_id];

        if ((in_front && !item.is_render_in_front()) ||
            (!in_front && item.is_render_in_front()))
        {
          continue;
        }

        const auto sprite_id = [&item]()
        {
          if (item.is_animated())
          {
            return item.get_sprite() + static_cast<int>((game_tick / 2) % item.get_sprite_count());
          }
          else
          {
            return item.get_sprite();
          }
        }();
        const auto src_rect = sprite_manager.get_rect_for_tile(sprite_id);
        const geometry::Rectangle dest_rect
        {
          (tile_x * 16) - game_camera.position.x(),
          (tile_y * 16) - game_camera.position.y(),
          16,
          16
        };
        game_surface->blit_surface(sprite_manager.get_surface(), src_rect, dest_rect, BlitType::CROP);
      }
    }
  }
}

void render_objects()
{
  // Render additional objects
  for (const auto& object : game->get_level().get_objects())
  {
    if (geometry::isColliding(geometry::Rectangle(object.position, object.size), game_camera))
    {
      const auto sprite_id = object.sprite_id + (game_tick % object.num_sprites);
      const auto src_rect = sprite_manager.get_rect_for_tile(sprite_id);
      const geometry::Rectangle dest_rect
      {
        object.position.x() - game_camera.position.x(),
        object.position.y() - game_camera.position.y(),
        object.size.x(),
        object.size.y()
      };
      game_surface->blit_surface(sprite_manager.get_surface(), src_rect, dest_rect, BlitType::CROP);
    }
  }
}

void render_game()
{
  // Update game camera
  // Note: this isn't exactly how the Crystal Caves camera work, but it's good enough
  const geometry::Position player_camera_relative_position {(game->get_player().position + (game->get_player().size / 2)) - (game_camera.position + (game_camera.size / 2))};
  if (player_camera_relative_position.x() < -4)
  {
    game_camera.position = geometry::Position(math::clamp(game_camera.position.x() + player_camera_relative_position.x() + 4,
                                                          0,
                                                          (game->get_level().get_tile_width() * 16) - CAMERA_SIZE.x()),
                                              game_camera.position.y());
  }
  else if (player_camera_relative_position.x() > 20)
  {
    game_camera.position = geometry::Position(math::clamp(game_camera.position.x() + player_camera_relative_position.x() - 20,
                                                          0,
                                                          (game->get_level().get_tile_width() * 16) - CAMERA_SIZE.x()),
                                              game_camera.position.y());
  }

  if (player_camera_relative_position.y() < -10)
  {
    game_camera.position = geometry::Position(game_camera.position.x(),
                                              math::clamp(game_camera.position.y() + player_camera_relative_position.y() + 10,
                                                          0,
                                                          (game->get_level().get_tile_height() * 16) - CAMERA_SIZE.y()));
  }
  else if (player_camera_relative_position.y() > 32)
  {
    game_camera.position = geometry::Position(game_camera.position.x(),
                                              math::clamp(game_camera.position.y() + player_camera_relative_position.y() - 32,
                                                          0,
                                                          (game->get_level().get_tile_height() * 16) - CAMERA_SIZE.y()));
  }

  // Clear game surface (background now)
  game_surface->fill_rect(geometry::Rectangle(0, 0, CAMERA_SIZE), { 33u, 33u, 33u });

  render_background();
  render_foreground(false);
  render_objects();
  render_player();
  render_foreground(true);
}

PlayerInput input_to_player_input(const Input& input)
{
  PlayerInput pi;
  pi.left = input.left.pressed;
  pi.right = input.right.pressed;
  pi.jump = input.z.pressed;
  pi.shoot = input.x.pressed;
  return pi;
}

int main()
{
  LOG_INFO("Starting!");

  // Create Window
  auto window = Window::create("OpenCrystalCaves", WINDOW_SIZE);
  if (!window)
  {
    return 1;
  }
  auto window_surface = window->get_surface();

  // Create game surface
  game_surface = window->create_surface(CAMERA_SIZE);
  if (!game_surface)
  {
    LOG_CRITICAL("Could not create game surface");
    return 1;
  }

  if (!sprite_manager.load_tileset("media/sprites.bmp"))
  {
    LOG_CRITICAL("Could not load tileset");
    return 1;
  }

  game = Game::create();
  if (!game || !game->init())
  {
    LOG_CRITICAL("Could not initialize Game");
    return 1;
  }

  // Set initial game camera
  game_camera = Camera(math::clamp(game->get_player().position.x() + (game->get_player().size.x() / 2) - (CAMERA_SIZE.x() / 2),
                                   0,
                                   (game->get_level().get_tile_width() * 16) - CAMERA_SIZE.x()),
                       math::clamp(game->get_player().position.y() + (game->get_player().size.y() / 2) - (CAMERA_SIZE.y() / 2),
                                   0,
                                   (game->get_level().get_tile_height() * 16) - CAMERA_SIZE.y()),
                       CAMERA_SIZE.x(),
                       CAMERA_SIZE.y());

  // Game loop
  {
    Input input;

    auto sdl_tick = SDL_GetTicks();
    game_tick = 0u;

    // Game loop logic
    const auto ms_per_update = 57;  // 17.5~ ticks per second
    auto tick_last_update = sdl_tick;
    auto lag = 0u;

    // FPS logic
    auto fps_num_renders = 0u;
    auto fps_last_calc = sdl_tick;
    auto fps_start_time = sdl_tick;
    auto fps = 0u;

    while (true)
    {
      /////////////////////////////////////////////////////////////////////////
      ///
      ///  Logic
      ///
      /////////////////////////////////////////////////////////////////////////
      sdl_tick = SDL_GetTicks();
      auto elapsed_ticks = sdl_tick - tick_last_update;
      tick_last_update = sdl_tick;
      lag += elapsed_ticks;
      while (lag >= ms_per_update)
      {
        // Read input
        read_input(&input);

        // Handle input
        if (input.quit)
        {
          return 0;  // Quit ASAP
        }
        if (input.num_1.pressed && !input.num_1.repeated)
        {
          debug = !debug;
        }
        if (input.enter.pressed && !input.enter.repeated)
        {
          paused = !paused;
        }

        if (!paused || (paused && input.space.pressed && !input.space.repeated))
        {
          // Call game loop
          game->update(game_tick, input_to_player_input(input));
          game_tick += 1;
        }

        lag -= ms_per_update;
      }

      /////////////////////////////////////////////////////////////////////////
      ///
      ///  Render
      ///
      /////////////////////////////////////////////////////////////////////////

      // Clear window surface
      window_surface->fill_rect(geometry::Rectangle(0, 0, WINDOW_SIZE), { 33u, 33u, 33u });

      // Render game
      render_game();

      // Render game surface to window surface, centered and scaled
      window_surface->blit_surface(game_surface.get(),
                                   geometry::Rectangle(0, 0, CAMERA_SIZE),
                                   geometry::Rectangle((WINDOW_SIZE - CAMERA_SIZE_SCALED) / 2, CAMERA_SIZE_SCALED),
                                   BlitType::SCALE);

      // Render FPS
      auto fps_str = "fps: " + std::to_string(fps);
      window_surface->render_text(geometry::Position(5, 5), fps_str, { 255u, 255u, 255u });

      // Debug
      if (debug)
      {
        // Put a black box where we're going to the draw the debug text
        window_surface->fill_rect({ 0, 24, 200, 165 }, { 0u, 0u, 0u });

        // Debug text
        const auto camera_position_str = "camera position: (" + std::to_string(game_camera.position.x()) + ", " + std::to_string(game_camera.position.y()) + ")";
        const auto player_position_str = "player position: (" + std::to_string(game->get_player().position.x()) + ", " + std::to_string(game->get_player().position.y()) + ")";
        const auto player_velocity_str = "player velocity: (" + std::to_string(game->get_player().velocity.x()) + ", " + std::to_string(game->get_player().velocity.y()) + ")";
        const auto player_walking_str  = std::string("player walking: ") + (game->get_player().walking ? "true" : "false");
        const auto player_jumping_str  = std::string("player jumping: ") + (game->get_player().jumping ? "true" : "false");
        const auto player_falling_str  = std::string("player falling: ") + (game->get_player().falling ? "true" : "false");
        const auto player_shooting_str = std::string("player shooting: ") + (game->get_player().shooting ? "true" : "false");
        const auto collide_str         = std::string("player collide: ")  + (game->get_player().collide_x ? "x " : "_ ") + (game->get_player().collide_y ? "y" : "_");

        window_surface->render_text(geometry::Position(5,  25), camera_position_str, { 255u, 0u, 0u });
        window_surface->render_text(geometry::Position(5,  45), player_position_str, { 255u, 0u, 0u });
        window_surface->render_text(geometry::Position(5,  65), player_velocity_str, { 255u, 0u, 0u });
        window_surface->render_text(geometry::Position(5,  85), player_walking_str,  { 255u, 0u, 0u });
        window_surface->render_text(geometry::Position(5, 105), player_jumping_str,  { 255u, 0u, 0u });
        window_surface->render_text(geometry::Position(5, 125), player_falling_str,  { 255u, 0u, 0u });
        window_surface->render_text(geometry::Position(5, 145), player_shooting_str, { 255u, 0u, 0u });
        window_surface->render_text(geometry::Position(5, 165), collide_str,         { 255u, 0u, 0u });

      }

      // Update screen
      window->refresh();

      // Calculate FPS each second
      fps_num_renders++;
      if (sdl_tick >= (fps_last_calc + 1000))
      {
        const auto total_time = sdl_tick - fps_start_time;
        fps = fps_num_renders / (total_time / 1000);
        fps_last_calc = sdl_tick;

        // Reset
        fps_num_renders = 0;
        fps_start_time = sdl_tick;
      }
    }
  }

  return 0;
}
