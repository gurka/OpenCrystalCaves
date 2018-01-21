#include "sg_engine_impl.h"

#include <cstdlib>
#include <memory>
#include <utility>

#include <SDL.h>
#include <SDL_ttf.h>

#include "input.h"
#include "graphics_impl.h"
#include "logger.h"

std::unique_ptr<SGEngine> SGEngine::create(const std::string& window_title, geometry::Size window_size)
{
  // Init SDL
  if (SDL_Init(SDL_INIT_VIDEO) < 0)
  {
    LOG_CRITICAL("Could not initialize SDL: %s", SDL_GetError());
    return std::unique_ptr<SGEngine>();
  }

  // Init SDL_ttf
  if (TTF_Init() < 0)
  {
    LOG_CRITICAL("Could not initialize TTF: %s", TTF_GetError());
    return std::unique_ptr<SGEngine>();
  }

  atexit(SDL_Quit);

  // Create window surface
  auto sdl_window = std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)>(SDL_CreateWindow(window_title.c_str(),
                                                                                               0,
                                                                                               0,
                                                                                               window_size.x(),
                                                                                               window_size.y(),
                                                                                               SDL_WINDOW_SHOWN),
                                                                              SDL_DestroyWindow);

  return std::make_unique<SGEngineImpl>(std::move(sdl_window));
}

std::unique_ptr<Surface> SGEngineImpl::get_window_surface()
{
  auto sdl_surface = std::unique_ptr<SDL_Surface, decltype(&SDL_FreeSurface)>(SDL_GetWindowSurface(sdl_window_.get()),
                                                                              [](SDL_Surface*){});
  return std::make_unique<SurfaceImpl>(std::move(sdl_surface));
}

std::unique_ptr<Surface> SGEngineImpl::create_surface(geometry::Size size)
{
  auto* window_surface = SDL_GetWindowSurface(sdl_window_.get());
  auto sdl_surface = std::unique_ptr<SDL_Surface, decltype(&SDL_FreeSurface)>(SDL_CreateRGBSurface(0,
                                                                                                   size.x(),
                                                                                                   size.y(),
                                                                                                   window_surface->format->BitsPerPixel,
                                                                                                   window_surface->format->Rmask,
                                                                                                   window_surface->format->Gmask,
                                                                                                   window_surface->format->Bmask,
                                                                                                   window_surface->format->Amask),
                                                                              SDL_FreeSurface);
  return std::make_unique<SurfaceImpl>(std::move(sdl_surface));
}

void SGEngineImpl::refresh()
{
  SDL_UpdateWindowSurface(sdl_window_.get());
}


unsigned SGEngineImpl::get_tick()
{
  return SDL_GetTicks();
}

void SGEngineImpl::poll_event(Input* input)
{
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
          return;  // Quit asap

        default:
          break;
      }
    }
  }
}
