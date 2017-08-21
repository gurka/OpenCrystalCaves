#ifndef SPRITEMGR_H_
#define SPRITEMGR_H_

#include <functional>
#include <memory>
#include <string>
#include <vector>

#include "SDL.h"
#include "geometry.h"

class SpriteManager
{
 public:
  SpriteManager()
    : tileset_(nullptr, SDL_FreeSurface)
  {
  }

  bool load_tileset(const std::string& filename);
  int number_of_tiles() const;
  SDL_Surface* get_surface() const;
  SDL_Rect get_rect_for_tile(int sprite) const;

 private:
  std::unique_ptr<SDL_Surface, decltype(&SDL_FreeSurface)> tileset_;
};

#endif  // SPRITEMGR_H_
