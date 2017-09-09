#include "sdl_wrapper_impl.h"

#include <cstdlib>
#include <memory>
#include <utility>

#include <SDL.h>
#include <SDL_ttf.h>

#include "logger.h"

std::unique_ptr<SDLWrapper> SDLWrapper::create()
{
  return std::make_unique<SDLWrapperImpl>();
}

bool SDLWrapperImpl::init()
{
  // Init SDL
  if (SDL_Init(SDL_INIT_VIDEO) < 0)
  {
    LOG_CRITICAL("Could not initialize SDL: %s", SDL_GetError());
    return false;
  }

  // Init SDL_ttf
  if (TTF_Init() < 0)
  {
    LOG_CRITICAL("Could not initialize TTF: %s", TTF_GetError());
    return false;
  }

  atexit(SDL_Quit);

  return true;
}

unsigned SDLWrapperImpl::get_tick()
{
  return SDL_GetTicks();
}
