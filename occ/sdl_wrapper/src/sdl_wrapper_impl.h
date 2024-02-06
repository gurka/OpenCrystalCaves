#ifndef SDL_WRAPPER_IMPL_H_
#define SDL_WRAPPER_IMPL_H_

#include "sdl_wrapper.h"

class SDLWrapperImpl : public SDLWrapper
{
 public:
  bool init() override;
  unsigned get_tick() override;
	void delay(const int ms) override;
};

#endif  // SDL_WRAPPER_IMPL_H_
