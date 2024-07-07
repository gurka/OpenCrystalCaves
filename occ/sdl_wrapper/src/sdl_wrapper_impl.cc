#include "sdl_wrapper_impl.h"

#include <cstdlib>
#include <memory>
#include <utility>

#include <SDL.h>

#include "logger.h"

std::unique_ptr<SDLWrapper> SDLWrapper::create()
{
  return std::make_unique<SDLWrapperImpl>();
}

bool SDLWrapperImpl::init()
{
  // Init SDL
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
  {
    LOG_CRITICAL("Could not initialize SDL: %s", SDL_GetError());
    return false;
  }

  atexit(SDL_Quit);

  return true;
}

unsigned SDLWrapperImpl::get_tick()
{
  return SDL_GetTicks();
}

void SDLWrapperImpl::delay(const int ms)
{
	return SDL_Delay(ms);
}
