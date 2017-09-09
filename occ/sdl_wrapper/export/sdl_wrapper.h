#ifndef SDL_WRAPPER_H_
#define SDL_WRAPPER_H_

#include <memory>

class SDLWrapper
{
 public:
  static std::unique_ptr<SDLWrapper> create();

  virtual ~SDLWrapper() = default;

  virtual bool init() = 0;
  virtual unsigned get_tick() = 0;
};

#endif  // SDL_WRAPPER_H_
