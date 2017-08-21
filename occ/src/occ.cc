#include <cassert>
#include <cstdio>

#include <memory>

#include <SDL.h>
#include <SDL_ttf.h>

#include "config.h"
#include "input.h"
#include "game.h"
#include "draw.h"

/////////////////////////////
//
// TODO: Logger class from gurka/gameserver
//
/////////////////////////////

using Window = std::unique_ptr<SDL_Window,  decltype(&SDL_DestroyWindow)>;

Window initSDL()
{
  // Init SDL
  if (SDL_Init(SDL_INIT_VIDEO) < 0)
  {
    fprintf(stderr, "Could not initialize SDL: %s\n", SDL_GetError());
    return Window(nullptr, SDL_DestroyWindow);
  }

  // Init SDL_ttf
  if (TTF_Init() < 0)
  {
    fprintf(stderr, "Could not initialize TTF: %s\n", TTF_GetError());
    return Window(nullptr, SDL_DestroyWindow);
  }

  atexit(SDL_Quit);

  return Window(SDL_CreateWindow("OpenCrystalCaves",
                                 0,
                                 0,
                                 config::CAMERA_WIDTH * config::SCREEN_SCALE,
                                 config::CAMERA_HEIGHT * config::SCREEN_SCALE,
                                 SDL_WINDOW_SHOWN),
                SDL_DestroyWindow);
}

void readInput(Input* input)
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

int main(int argc, char* argv[])
{
  Window window = initSDL();
  if (!window)
  {
    return 1;
  }
  auto* window_surface = SDL_GetWindowSurface(window.get());

  Game game;
  if (!game.init(window_surface))
  {
    fprintf(stderr, "Could not initialize Game\n");
    return 1;
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
        readInput(&input);
        game.update(input);
        lag -= ms_per_update;
      }
      if (input.quit)
      {
        break;
      }

      /////////////////////////////////////////////////////////////////////////
      ///
      ///  Render
      ///
      /////////////////////////////////////////////////////////////////////////
      // Render game
      game.render(window_surface);

      // Render FPS
      auto fps_str = "fps: " + std::to_string(fps);
      draw::text(5, 5, fps_str, { 255u, 255u, 255u }, window_surface);

      // Update screen
      SDL_UpdateWindowSurface(window.get());

      fps_num_renders++;

      // Calculate FPS each second
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

  return 0;
}
