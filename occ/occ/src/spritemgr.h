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
    : sprite_surface_(), char_surface_()
  {
  }

  bool load_tilesets(Window& window, const int episode);
  const Surface* get_surface() const;
  geometry::Rectangle get_rect_for_tile(const int sprite) const;
	const Surface* get_char_surface() const;
 geometry::Rectangle get_rect_for_char(const int ch) const;

 private:
  std::unique_ptr<Surface> sprite_surface_;
	std::unique_ptr<Surface> char_surface_;
};

#endif  // SPRITEMGR_H_
