#include "sdl_wrapper_impl.h"

#include <cstdlib>
#include <memory>
#include <utility>

#include <SDL.h>
#include <SDL_ttf.h>

#include "graphics_impl.h"
#include "logger.h"

std::unique_ptr<SDLWrapper> SDLWrapper::create(const std::string& window_title, geometry::Size window_size)
{
  // Init SDL
  if (SDL_Init(SDL_INIT_VIDEO) < 0)
  {
    LOG_CRITICAL("Could not initialize SDL: %s", SDL_GetError());
    return std::unique_ptr<SDLWrapper>();
  }

  // Init SDL_ttf
  if (TTF_Init() < 0)
  {
    LOG_CRITICAL("Could not initialize TTF: %s", TTF_GetError());
    return std::unique_ptr<SDLWrapper>();
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

  return std::make_unique<SDLWrapperImpl>(std::move(sdl_window));
}

std::unique_ptr<Surface> SDLWrapperImpl::get_window_surface()
{
  auto sdl_surface = std::unique_ptr<SDL_Surface, decltype(&SDL_FreeSurface)>(SDL_GetWindowSurface(sdl_window_.get()),
                                                                              [](SDL_Surface*){});
  return std::make_unique<SurfaceImpl>(std::move(sdl_surface));
}

std::unique_ptr<Surface> SDLWrapperImpl::create_surface(geometry::Size size)
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

void SDLWrapperImpl::refresh()
{
  SDL_UpdateWindowSurface(sdl_window_.get());
}


unsigned SDLWrapperImpl::get_tick()
{
  return SDL_GetTicks();
}
