#ifndef SG_ENGINE_H_
#define SG_ENGINE_H_

#include <memory>
#include <string>

#include "geometry.h"

class Surface;
struct Input;

class SGEngine
{
 public:
  static std::unique_ptr<SGEngine> create(const std::string& window_title,
                                          geometry::Size window_size);

  virtual ~SGEngine() = default;

  virtual std::unique_ptr<Surface> get_window_surface() = 0;
  virtual std::unique_ptr<Surface> create_surface(geometry::Size size) = 0;

  virtual void refresh() = 0;
  virtual unsigned get_tick() = 0;
  virtual void poll_event(Input* input) = 0;
};

#endif  // SG_ENGINE_H_
