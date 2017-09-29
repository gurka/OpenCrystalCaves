#ifndef SPRITEMGR_H_
#define SPRITEMGR_H_

#include <memory>
#include <string>

#include "graphics.h"
#include "geometry.h"

// TODO: Rename files to sprite_manager.cc/h ?

class SpriteManager
{
 public:
  SpriteManager()
    : sprite_surface_()
  {
  }

  bool load_tileset(const std::string& filename);
  int number_of_tiles() const;
  const Surface* get_surface() const;
  geometry::Rectangle get_rect_for_tile(int sprite) const;

 private:
  std::unique_ptr<Surface> sprite_surface_;
};

#endif  // SPRITEMGR_H_
