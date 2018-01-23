#ifndef SG_ENGINE_H_
#define SG_ENGINE_H_

#include <memory>
#include <string>

#include "geometry.h"

class Surface;
struct Input;
class SGEngineSettings;

class SGEngine
{
 public:
  using GameTickFunc = void(SGEngineSettings*, unsigned, const Input&);
  using RenderFunc = void(unsigned);

  static std::unique_ptr<SGEngine> create(const std::string& window_title,
                                          geometry::Size window_size);

  virtual ~SGEngine() = default;

  virtual std::unique_ptr<Surface> get_window_surface() = 0;
  virtual std::unique_ptr<Surface> create_surface(geometry::Size size) = 0;

  virtual SGEngineSettings& get_settings() = 0;

  virtual void run(const std::function<GameTickFunc>& game_tick_func,
                   const std::function<RenderFunc>& render_func) = 0;
};

class SGEngineSettings
{
 public:
  virtual ~SGEngineSettings() = default;

  virtual void toggle_show_fps() = 0;
  virtual void quit() = 0;
};

#endif  // SG_ENGINE_H_
