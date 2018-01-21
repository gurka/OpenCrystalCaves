#ifndef SDL_WRAPPER_IMPL_H_
#define SDL_WRAPPER_IMPL_H_

#include "sdl_wrapper.h"

#include <memory>
#include <string>
#include <utility>

#include <SDL.h>

#include "geometry.h"

class Surface;

class SDLWrapperImpl : public SDLWrapper
{
 public:
  SDLWrapperImpl(std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)> sdl_window)
    : sdl_window_(std::move(sdl_window))
  {
  }

  std::unique_ptr<Surface> get_window_surface() override;
  std::unique_ptr<Surface> create_surface(geometry::Size size) override;

  void refresh() override;
  unsigned get_tick() override;

 private:
  std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)> sdl_window_;
};

#endif  // SDL_WRAPPER_IMPL_H_
