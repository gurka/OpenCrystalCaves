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

static constexpr int CAMERA_WIDTH  = 320;
static constexpr int CAMERA_HEIGHT = 192;
static constexpr int SCREEN_SCALE = 3;

using Window = std::unique_ptr<SDL_Window,  decltype(&SDL_DestroyWindow)>;

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
                                 CAMERA_WIDTH * SCREEN_SCALE,
                                 CAMERA_HEIGHT * SCREEN_SCALE,
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
          if (event.type == SDL_KEYDOWN)
          {
            input->up.pressed  = true;
          }
          else
          {
            input->up.pressed = false;
            input->up.repeated = false;
          }
          break;

        case SDLK_DOWN:
          if (event.type == SDL_KEYDOWN)
          {
            input->down.pressed  = true;
          }
          else
          {
            input->down.pressed = false;
            input->down.repeated = false;
          }
          break;

        case SDLK_LEFT:
          if (event.type == SDL_KEYDOWN)
          {
            input->left.pressed  = true;
          }
          else
          {
            input->left.pressed = false;
            input->left.repeated = false;
          }
          break;

        case SDLK_RIGHT:
          if (event.type == SDL_KEYDOWN)
          {
            input->right.pressed  = true;
          }
          else
          {
            input->right.pressed = false;
            input->right.repeated = false;
          }
          break;

        case SDLK_SPACE:
          if (event.type == SDL_KEYDOWN)
          {
            input->space.pressed  = true;
          }
          else
          {
            input->space.pressed = false;
            input->space.repeated = false;
          }
          break;

        case SDLK_1:
          if (event.type == SDL_KEYDOWN)
          {
            input->num_1.pressed  = true;
          }
          else
          {
            input->num_1.pressed = false;
            input->num_1.repeated = false;
          }
          break;

        case SDLK_2:
          if (event.type == SDL_KEYDOWN)
          {
            input->num_2.pressed  = true;
          }
          else
          {
            input->num_2.pressed = false;
            input->num_2.repeated = false;
          }
          break;

        case SDLK_3:
          if (event.type == SDL_KEYDOWN)
          {
            input->num_3.pressed  = true;
          }
          else
          {
            input->num_3.pressed = false;
            input->num_3.repeated = false;
          }
          break;

        case SDLK_4:
          if (event.type == SDL_KEYDOWN)
          {
            input->num_4.pressed  = true;
          }
          else
          {
            input->num_4.pressed = false;
            input->num_4.repeated = false;
          }
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

void render_game(const Game& game,
                 const SpriteManager& sprite_manager,
                 SDL_Surface* game_surface,
                 unsigned tick,
                 geometry::Rectangle* camera_out,
                 bool draw_aabbs)
{
  // Get game info
  const auto& player = game.get_player();
  const auto& items = game.get_items();
  const auto& level = game.get_level();

  // Calculate where the camera is
  const geometry::Position camera_pos(player.position +  // player position
                                      geometry::Position(8, 8) -  // plus half player size
                                      geometry::Position(CAMERA_WIDTH / 2, CAMERA_HEIGHT / 2));  // minus half camera size

  // Create a Rectangle that represents the camera and adjust the position so that nothing outside the level is visible
  const geometry::Rectangle camera(math::clamp(camera_pos.getX(),
                                               0,
                                               (level.get_tile_width() * 16) - CAMERA_WIDTH),
                                   math::clamp(camera_pos.getY(),
                                               0,
                                               (level.get_tile_height() * 16) - CAMERA_HEIGHT),
                                   CAMERA_WIDTH,
                                   CAMERA_HEIGHT);
  if (camera_out)
  {
      *camera_out = camera;
  }

  // Clear game surface (background now)
  SDL_FillRect(game_surface, nullptr, SDL_MapRGB(game_surface->format, 33, 33, 33));

  // This is the visible part of the level
  const auto start_tile_x = camera.position.getX() > 0 ? camera.position.getX() / 16 : 0;
  const auto start_tile_y = camera.position.getY() > 0 ? camera.position.getY() / 16 : 0;
  const auto end_tile_x = (camera.position.getX() + camera.size.getX()) / 16;
  const auto end_tile_y = (camera.position.getY() + camera.size.getY()) / 16;

  // SpriteSheet surface
  auto sprite_surface = sprite_manager.get_surface();

  // Render all background tiles
  {
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
            (tile_x * 16) - camera.position.getX(),
            (tile_y * 16) - camera.position.getY(),
            16,
            16
          };
          SDL_BlitSurface(sprite_surface, &src_rect, game_surface, &dest_rect);
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
        auto item_id = level.get_tile_middleground(tile_x, tile_y);
        if (item_id != Item::invalid)
        {
          const auto& item = items[item_id];
          const auto sprite_id = item.get_sprite();
          auto src_rect = sprite_manager.get_rect_for_tile(sprite_id);
          SDL_Rect dest_rect
          {
            (tile_x * 16) - camera.position.getX(),
            (tile_y * 16) - camera.position.getY(),
            16,
            16
          };
          SDL_BlitSurface(sprite_surface, &src_rect, game_surface, &dest_rect);
        }
      }
    }
  }

  // Render player
  {
    SDL_Rect src_rect = [&player, &sprite_manager]()
    {
      if (player.direction == Player::Direction::right)
      {
        if (player.state == Player::State::still)
        {
          return sprite_manager.get_rect_for_tile(player.sprite_standing_right);
        }
        else if (player.state == Player::State::walking)
        {
          return sprite_manager.get_rect_for_tile(player.sprite_walking_right[player.animation_tick % player.sprite_walking_right.size()]);
        }
        else  // player_.state == Player::State::jumping
        {
          return sprite_manager.get_rect_for_tile(player.sprite_jumping_right);
        }
      }
      else  // player_.direction == Player::Direction::left
      {
        if (player.state == Player::State::still)
        {
          return sprite_manager.get_rect_for_tile(player.sprite_standing_left);
        }
        else if (player.state == Player::State::walking)
        {
          return sprite_manager.get_rect_for_tile(player.sprite_walking_left[player.animation_tick % player.sprite_walking_left.size()]);
        }
        else  // player_.state == Player::State::jumping
        {
          return sprite_manager.get_rect_for_tile(player.sprite_jumping_left);
        }
      }
    }();
    auto player_render_pos = player.position - camera.position;
    SDL_Rect dest_rect { player_render_pos.getX(), player_render_pos.getY(), 16, 16 };
    SDL_BlitSurface(sprite_surface, &src_rect, game_surface, &dest_rect);
  }

  // Render all foreground items visible in the level
  {
    for (int tile_y = start_tile_y; tile_y <= end_tile_y; tile_y++)
    {
      for (int tile_x = start_tile_x; tile_x <= end_tile_x; tile_x++)
      {
        auto item_id = level.get_tile_foreground(tile_x, tile_y);
        if (item_id != Item::invalid)
        {
          const auto& item = items[item_id];
          const auto sprite_id = [&item](unsigned tick)
          {
            if (item.is_animated())
            {
              return item.get_sprite() + static_cast<int>((tick / 128u) % item.get_sprite_count());
            }
            else
            {
              return item.get_sprite();
            }
          }(tick);
          auto src_rect = sprite_manager.get_rect_for_tile(sprite_id);
          SDL_Rect dest_rect
          {
            (tile_x * 16) - camera.position.getX(),
            (tile_y * 16) - camera.position.getY(),
            16,
            16
          };
          SDL_BlitSurface(sprite_surface, &src_rect, game_surface, &dest_rect);
        }
      }
    }
  }

  // Debug
  if (draw_aabbs)
  {
    for (const auto& aabb : game.get_level().get_aabbs())
    {
      if (geometry::isColliding(camera, aabb))
      {
        // Adjust the aabb position based on camera and render it
        draw::rectangle(geometry::Rectangle(aabb.position - camera.position, aabb.size),
                        { 255u, 0u, 0u, 0u },
                        game_surface);
      }
    }
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
  std::unique_ptr<SDL_Surface, decltype(&SDL_FreeSurface)> game_surface(nullptr, SDL_FreeSurface);
  game_surface.reset(SDL_CreateRGBSurface(0,
                                          CAMERA_WIDTH,
                                          CAMERA_HEIGHT,
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
  SDL_SetColorKey(game_surface.get(), SDL_TRUE, SDL_MapRGB(game_surface->format, 0, 180, 0));

  SpriteManager sprite_manager;
  if (!sprite_manager.load_tileset("media/sprites.bmp"))
  {
    LOG_CRITICAL("Could not load tileset");
    return EXIT_FAILURE;
  }

  Game game;
  if (!game.init())
  {
    LOG_CRITICAL("Could not initialize Game");
    return EXIT_FAILURE;
  }

  // Game loop
  {
    Input input;

    const auto current_tick = SDL_GetTicks();

    // Game loop logic
    const auto ms_per_update = 57;  // 17.5~ ticks per second
    auto tick_last_update = current_tick;
    auto lag = 0u;

    // FPS logic
    auto fps_num_renders = 0u;
    auto fps_last_calc = current_tick;
    auto fps_start_time = current_tick;
    auto fps = 0u;

    // Debug information
    bool debug = false;
    geometry::Rectangle game_camera;
    bool draw_aabbs = false;

    while (true)
    {
      /////////////////////////////////////////////////////////////////////////
      ///
      ///  Logic
      ///
      /////////////////////////////////////////////////////////////////////////
      auto current_tick = SDL_GetTicks();
      auto elapsed_ticks = current_tick - tick_last_update;
      tick_last_update = current_tick;
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
          draw_aabbs = !draw_aabbs;
        }
        if (input.num_2.pressed && !input.num_2.repeated)
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

      // Render game
      render_game(game, sprite_manager, game_surface.get(), current_tick, debug ? &game_camera : nullptr, draw_aabbs);

      // Render game surface to window surface (scaled)
      SDL_Rect src_rect = { 0, 0, CAMERA_WIDTH, CAMERA_HEIGHT };
      SDL_Rect dest_rect = { 0, 0, CAMERA_WIDTH * SCREEN_SCALE, CAMERA_HEIGHT * SCREEN_SCALE };
      SDL_BlitScaled(game_surface.get(), &src_rect, window_surface, &dest_rect);

      // Render FPS
      auto fps_str = "fps: " + std::to_string(fps);
      draw::text(5, 5, fps_str, { 255u, 255u, 255u, 0u }, window_surface);

      // Debug
      if (debug)
      {
        // Debug info
        const auto camera_str  = "camera pixel: (" + std::to_string(game_camera.position.getX()) + ", " + std::to_string(game_camera.position.getY()) + ")";
        const auto pixel_str   = "player pixel: (" + std::to_string(game.get_player().position.getX()) + ", " + std::to_string(game.get_player().position.getY()) + ")";
        const auto tiles_str   = "player tiles: (" + std::to_string(game.get_player().position.getX() / 16) + ", " + std::to_string(game.get_player().position.getY() / 16) + ")";
        const auto collide_str = std::string("collide: ") + (game.player_collide_x() ? "x " : "_ ") + (game.player_collide_y() ? "y" : "_");

        draw::text(5,  25, camera_str,  { 255u, 0u, 0u, 0u}, window_surface);
        draw::text(5,  45, pixel_str,   { 255u, 0u, 0u, 0u}, window_surface);
        draw::text(5,  65, tiles_str,   { 255u, 0u, 0u, 0u}, window_surface);
        draw::text(5,  85, collide_str, { 255u, 0u, 0u, 0u}, window_surface);

      }

      // Update screen
      SDL_UpdateWindowSurface(window.get());

      // Calculate FPS each second
      fps_num_renders++;
      if (current_tick >= (fps_last_calc + 1000))
      {
        const auto total_time = current_tick - fps_start_time;
        fps = fps_num_renders / (total_time / 1000);
        fps_last_calc = current_tick;

        // Reset
        fps_num_renders = 0;
        fps_start_time = current_tick;
      }
    }
  }

  return EXIT_SUCCESS;
}
