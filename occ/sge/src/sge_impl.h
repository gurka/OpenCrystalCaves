#ifndef SG_ENGINE_IMPL_H_
#define SG_ENGINE_IMPL_H_

#include "sge.h"

#include <memory>
#include <string>
#include <utility>

#include <SDL.h>

#include "geometry.h"

class Surface;
struct Input;

class SGEngineImpl : public SGEngine,
                     public SGEngineSettings
{
 public:
  SGEngineImpl(std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)> sdl_window)
    : sdl_window_(std::move(sdl_window)),
      sdl_window_surface_(get_window_surface()),
      engine_tick_(0u),
      show_fps_(false),
      quit_(false)
  {
  }

  // From SGEngine
  std::unique_ptr<Surface> get_window_surface() override;
  std::unique_ptr<Surface> create_surface(geometry::Size size) override;

  SGEngineSettings& get_settings() { return *this; }

  void run(const std::function<GameTickFunc>& game_tick_func,
           const std::function<RenderFunc>& render_func) override;

  // From SGEngineSettings
  void toggle_show_fps() override { show_fps_ = !show_fps_; }
  void quit() override { quit_ = true; }

 private:
  void poll_event(Input* input);

  std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)> sdl_window_;
  std::unique_ptr<Surface> sdl_window_surface_;
  unsigned engine_tick_;
  bool show_fps_;
  bool quit_;
};

#endif  // SG_ENGINE_IMPL_H_
