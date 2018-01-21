#ifndef SDL_WRAPPER_H_
#define SDL_WRAPPER_H_

#include <memory>
#include <string>

#include "geometry.h"

class Surface;

class SDLWrapper
{
 public:
  static std::unique_ptr<SDLWrapper> create(const std::string& window_title,
                                            geometry::Size window_size);

  virtual ~SDLWrapper() = default;

  virtual std::unique_ptr<Surface> get_window_surface() = 0;
  virtual std::unique_ptr<Surface> create_surface(geometry::Size size) = 0;

  virtual void refresh() = 0;
  virtual unsigned get_tick() = 0;
};

#endif  // SDL_WRAPPER_H_
