#include <cassert>
#include <cstdio>

#include <memory>

#include <SDL.h>
#include <SDL_ttf.h>

#include "input.h"
#include "spritemgr.h"
#include "logger.h"
#include "draw.h"
#include "game.h"
#include "player_input.h"

using Window = std::unique_ptr<SDL_Window,  decltype(&SDL_DestroyWindow)>;
using Camera = geometry::Rectangle;

// The size of the game camera - not scaled
static constexpr geometry::Size CAMERA_SIZE = geometry::Size(320, 192);

// The size of the game camera - scaled 3x
static constexpr geometry::Size CAMERA_SIZE_SCALED = CAMERA_SIZE * 3;

// The size of the game window (camera size scaled + 300 pixels on both axis)
static constexpr geometry::Size WINDOW_SIZE = CAMERA_SIZE_SCALED + geometry::Size(300, 300);

// Game variables
static Game game;
static SpriteManager sprite_manager;

// Used by the render_x functions and should be up to date before each call to render_x()
static std::unique_ptr<SDL_Surface, decltype(&SDL_FreeSurface)> game_surface(nullptr, &SDL_FreeSurface);
static Camera game_camera;
static unsigned game_tick;

// Debug information
static bool debug(false);

Window init_SDL()
{
  // Init SDL
  if (SDL_Init(SDL_INIT_VIDEO) < 0)
  {
    LOG_CRITICAL("Could not initialize SDL: %s", SDL_GetError());
    return Window(nullptr, SDL_DestroyWindow);
  }

  // Init SDL_ttf
  if (TTF_Init() < 0)
  {
    LOG_CRITICAL("Could not initialize TTF: %s", TTF_GetError());
    return Window(nullptr, SDL_DestroyWindow);
  }

  atexit(SDL_Quit);

  return Window(SDL_CreateWindow("OpenCrystalCaves",
                                 0,
                                 0,
                                 WINDOW_SIZE.x(),
                                 WINDOW_SIZE.y(),
                                 SDL_WINDOW_SHOWN),
                SDL_DestroyWindow);
}

void read_input(Input* input)
{
  assert(input != nullptr);

  // Set any input that is pressed as repeated here
  input->up.repeated    = input->up.pressed;
  input->down.repeated  = input->down.pressed;
  input->left.repeated  = input->left.pressed;
  input->right.repeated = input->right.pressed;
  input->space.repeated = input->space.pressed;
  input->num_1.repeated = input->num_1.pressed;
  input->num_2.repeated = input->num_2.pressed;
  input->num_3.repeated = input->num_3.pressed;
  input->num_4.repeated = input->num_4.pressed;

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

        case SDLK_SPACE:
          input->space.pressed = event.type == SDL_KEYDOWN;
          if (!input->space.pressed) input->space.repeated = false;
          break;

        case SDLK_1:
          input->num_1.pressed = event.type == SDL_KEYDOWN;
          if (!input->num_1.pressed) input->num_1.repeated = false;
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
  const auto& level = game.get_level();
  const auto& items = game.get_items();

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
        auto src_rect = sprite_manager.get_rect_for_tile(sprite_id);
        SDL_Rect dest_rect
        {
          (tile_x * 16) - game_camera.position.x(),
          (tile_y * 16) - game_camera.position.y(),
          16,
          16
        };
        SDL_BlitSurface(sprite_manager.get_surface(), &src_rect, game_surface.get(), &dest_rect);
      }
    }
  }
}

void render_middleground()
{
  const auto& level = game.get_level();
  const auto& items = game.get_items();

  const auto start_tile_x = game_camera.position.x() > 0 ? game_camera.position.x() / 16 : 0;
  const auto start_tile_y = game_camera.position.y() > 0 ? game_camera.position.y() / 16 : 0;
  const auto end_tile_x = (game_camera.position.x() + game_camera.size.x()) / 16;
  const auto end_tile_y = (game_camera.position.y() + game_camera.size.y()) / 16;

  for (int tile_y = start_tile_y; tile_y <= end_tile_y; tile_y++)
  {
    for (int tile_x = start_tile_x; tile_x <= end_tile_x; tile_x++)
    {
      auto item_id = level.get_tile_middleground(tile_x, tile_y);
      if (item_id != Item::invalid)
      {
        const auto& item = items[item_id];
        const auto sprite_id = item.get_sprite();
        auto src_rect = sprite_manager.get_rect_for_tile(sprite_id);
        SDL_Rect dest_rect
        {
          (tile_x * 16) - game_camera.position.x(),
          (tile_y * 16) - game_camera.position.y(),
          16,
          16
        };
        SDL_BlitSurface(sprite_manager.get_surface(), &src_rect, game_surface.get(), &dest_rect);
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

  static constexpr int sprite_standing_left = 272;
  static constexpr std::array<int, 12> sprite_walking_left =
  {
      272, 273, 274, 275, 276, 277, 278, 279, 280, 281, 282, 283
  };
  static constexpr int sprite_jumping_left = 285;

  const auto& player = game.get_player();

  SDL_Rect src_rect = [&player]()
  {
    if (player.direction == Player::Direction::right)
    {
      if (player.state == Player::State::still)
      {
        return sprite_manager.get_rect_for_tile(sprite_standing_right);
      }
      else if (player.state == Player::State::walking)
      {
        return sprite_manager.get_rect_for_tile(sprite_walking_right[player.animation_tick % sprite_walking_right.size()]);
      }
      else  // player_.state == Player::State::jumping
      {
        return sprite_manager.get_rect_for_tile(sprite_jumping_right);
      }
    }
    else  // player_.direction == Player::Direction::left
    {
      if (player.state == Player::State::still)
      {
        return sprite_manager.get_rect_for_tile(sprite_standing_left);
      }
      else if (player.state == Player::State::walking)
      {
        return sprite_manager.get_rect_for_tile(sprite_walking_left[player.animation_tick % sprite_walking_left.size()]);
      }
      else  // player_.state == Player::State::jumping
      {
        return sprite_manager.get_rect_for_tile(sprite_jumping_left);
      }
    }
  }();
  auto player_render_pos = player.position - game_camera.position;

  // Note: player size is 12x16 but the sprite is 16x16 so we need to adjust where
  // the player is rendered
  SDL_Rect dest_rect { player_render_pos.x() - 2, player_render_pos.y(), 16, 16 };

  SDL_BlitSurface(sprite_manager.get_surface(), &src_rect, game_surface.get(), &dest_rect);
}

void render_foreground()
{
  const auto& level = game.get_level();
  const auto& items = game.get_items();

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
        const auto sprite_id = [&item]()
        {
          if (item.is_animated())
          {
            return item.get_sprite() + static_cast<int>((game_tick / 128u) % item.get_sprite_count());
          }
          else
          {
            return item.get_sprite();
          }
        }();
        auto src_rect = sprite_manager.get_rect_for_tile(sprite_id);
        SDL_Rect dest_rect
        {
          (tile_x * 16) - game_camera.position.x(),
          (tile_y * 16) - game_camera.position.y(),
          16,
          16
        };
        SDL_BlitSurface(sprite_manager.get_surface(), &src_rect, game_surface.get(), &dest_rect);
      }
    }
  }
}

void render_debug()
{
  // Render a red rectangle around every AABB
  for (const auto& aabb : game.get_level().get_aabbs())
  {
    if (geometry::isColliding(game_camera, aabb))
    {
      // Adjust the aabb position based on camera and render it
      draw::rectangle(geometry::Rectangle(aabb.position - game_camera.position, aabb.size),
                      { 255u, 0u, 0u, 0u },
                      game_surface.get());
    }
  }

  // Render a red rectangle around the player
  draw::rectangle(geometry::Rectangle(game.get_player().position - game_camera.position, game.get_player().size),
                  { 255u, 0u, 0u, 0u },
                  game_surface.get());
}

void render_game()
{
  // Get game info
  const auto& player = game.get_player();
  const auto& level = game.get_level();

  // Calculate where the camera is
  const geometry::Position camera_pos(player.position +  // player position
                                      geometry::Position(8, 8) -  // plus half player size
                                      geometry::Position(CAMERA_SIZE.x() / 2, CAMERA_SIZE.y() / 2));  // minus half camera size

  // Create a Rectangle that represents the camera and adjust the position so that nothing outside the level is visible
  game_camera = Camera(math::clamp(camera_pos.x(),
                                   0,
                                   (level.get_tile_width() * 16) - CAMERA_SIZE.x()),
                       math::clamp(camera_pos.y(),
                                   0,
                                   (level.get_tile_height() * 16) - CAMERA_SIZE.y()),
                       CAMERA_SIZE.x(),
                       CAMERA_SIZE.y());

  // Clear game surface (background now)
  SDL_FillRect(game_surface.get(), nullptr, SDL_MapRGB(game_surface->format, 33, 33, 33));

  render_background();
  render_middleground();
  render_player();
  render_foreground();
  if (debug)
  {
    render_debug();
  }
}

PlayerInput input_to_player_input(const Input& input)
{
  PlayerInput pi;
  pi.left = input.left.pressed;
  pi.right = input.right.pressed;
  pi.jump = input.space.pressed;  // TODO should be CTRL
  pi.shoot = false;  // TODO should be ALT or space (?)
  return pi;
}

int main()
{
  LOG_INFO("Starting!");

  // Create Window
  Window window = init_SDL();
  if (!window)
  {
    return EXIT_FAILURE;
  }
  auto* window_surface = SDL_GetWindowSurface(window.get());

  // Create game surface
  game_surface.reset(SDL_CreateRGBSurface(0,
                                          CAMERA_SIZE.x(),
                                          CAMERA_SIZE.y(),
                                          window_surface->format->BitsPerPixel,
                                          window_surface->format->Rmask,
                                          window_surface->format->Gmask,
                                          window_surface->format->Bmask,
                                          window_surface->format->Amask));
  if (!game_surface)
  {
    LOG_CRITICAL("Could not create game surface");
    return EXIT_FAILURE;
  }

  if (!sprite_manager.load_tileset("media/sprites.bmp"))
  {
    LOG_CRITICAL("Could not load tileset");
    return EXIT_FAILURE;
  }

  if (!game.init())
  {
    LOG_CRITICAL("Could not initialize Game");
    return EXIT_FAILURE;
  }

  // Game loop
  {
    Input input;

    game_tick = SDL_GetTicks();

    // Game loop logic
    const auto ms_per_update = 57;  // 17.5~ ticks per second
    auto tick_last_update = game_tick;
    auto lag = 0u;

    // FPS logic
    auto fps_num_renders = 0u;
    auto fps_last_calc = game_tick;
    auto fps_start_time = game_tick;
    auto fps = 0u;

    while (true)
    {
      /////////////////////////////////////////////////////////////////////////
      ///
      ///  Logic
      ///
      /////////////////////////////////////////////////////////////////////////
      game_tick = SDL_GetTicks();
      auto elapsed_ticks = game_tick - tick_last_update;
      tick_last_update = game_tick;
      lag += elapsed_ticks;
      while (lag >= ms_per_update)
      {
        // Read input
        read_input(&input);

        // Handle input
        if (input.quit)
        {
          return EXIT_SUCCESS;  // Quit ASAP
        }
        if (input.num_1.pressed && !input.num_1.repeated)
        {
          debug = !debug;
        }

        // Call game loop
        game.update(input_to_player_input(input));

        lag -= ms_per_update;
      }

      /////////////////////////////////////////////////////////////////////////
      ///
      ///  Render
      ///
      /////////////////////////////////////////////////////////////////////////

      // Clear window surface
      SDL_FillRect(window_surface, nullptr, SDL_MapRGB(window_surface->format, 33, 33, 33));

      // Render game
      render_game();

      // Render game surface to window surface, centered and scaled
      SDL_Rect src_rect = { 0, 0, CAMERA_SIZE.x(), CAMERA_SIZE.y() };
      SDL_Rect dest_rect = {
          (WINDOW_SIZE.x() - CAMERA_SIZE_SCALED.x()) / 2,
          (WINDOW_SIZE.y() - CAMERA_SIZE_SCALED.y()) / 2,
          CAMERA_SIZE_SCALED.x(),
          CAMERA_SIZE_SCALED.y()
      };
      SDL_BlitScaled(game_surface.get(), &src_rect, window_surface, &dest_rect);

      // Render FPS
      auto fps_str = "fps: " + std::to_string(fps);
      draw::text(5, 5, fps_str, { 255u, 255u, 255u, 0u }, window_surface);

      // Debug
      if (debug)
      {
        // Debug info
        const auto camera_pos_str = "camera position: (" + std::to_string(game_camera.position.x()) + ", " + std::to_string(game_camera.position.y()) + ")";
        const auto player_pos_str = "player position: (" + std::to_string(game.get_player().position.x()) + ", " + std::to_string(game.get_player().position.y()) + ")";
        const auto player_vel_str = "player velocity: (" + std::to_string(game.get_player().velocity.x()) + ", " + std::to_string(game.get_player().velocity.y()) + ")";
        const auto collide_str    = std::string("collide: ") + (game.player_collide_x() ? "x " : "_ ") + (game.player_collide_y() ? "y" : "_");

        draw::text(5,  25, camera_pos_str, { 255u, 0u, 0u, 0u}, window_surface);
        draw::text(5,  45, player_pos_str, { 255u, 0u, 0u, 0u}, window_surface);
        draw::text(5,  65, player_vel_str, { 255u, 0u, 0u, 0u}, window_surface);
        draw::text(5,  85, collide_str,    { 255u, 0u, 0u, 0u}, window_surface);

      }

      // Update screen
      SDL_UpdateWindowSurface(window.get());

      // Calculate FPS each second
      fps_num_renders++;
      if (game_tick >= (fps_last_calc + 1000))
      {
        const auto total_time = game_tick - fps_start_time;
        fps = fps_num_renders / (total_time / 1000);
        fps_last_calc = game_tick;

        // Reset
        fps_num_renders = 0;
        fps_start_time = game_tick;
      }
    }
  }

  return EXIT_SUCCESS;
}
