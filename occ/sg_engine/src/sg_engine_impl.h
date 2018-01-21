#ifndef SG_ENGINE_IMPL_H_
#define SG_ENGINE_IMPL_H_

#include "sg_engine.h"

#include <memory>
#include <string>
#include <utility>

#include <SDL.h>

#include "geometry.h"

class Surface;
struct Input;

class SGEngineImpl : public SGEngine
{
 public:
  SGEngineImpl(std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)> sdl_window)
    : sdl_window_(std::move(sdl_window))
  {
  }

  std::unique_ptr<Surface> get_window_surface() override;
  std::unique_ptr<Surface> create_surface(geometry::Size size) override;

  void refresh() override;
  unsigned get_tick() override;
  void poll_event(Input* input) override;

 private:
  std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)> sdl_window_;
};

#endif  // SG_ENGINE_IMPL_H_
