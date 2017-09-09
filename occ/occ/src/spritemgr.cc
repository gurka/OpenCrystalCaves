#include "spritemgr.h"

#include "logger.h"

bool SpriteManager::load_tileset(const std::string& filename)
{
  // Check if tileset already loaded
  if (sprite_surface_)
  {
    return false;
  }

  // Load tileset
  sprite_surface_ = Surface::from_bmp(filename.c_str());
  if (!sprite_surface_)
  {
    LOG_CRITICAL("Could not load '%s'", filename.c_str());
    return false;
  }

  return true;
}

int SpriteManager::number_of_tiles() const
{
  return (sprite_surface_->width() / 16) * (sprite_surface_->height() / 16);
}

const Surface* SpriteManager::get_surface() const
{
  return sprite_surface_.get();
}

geometry::Rectangle SpriteManager::get_rect_for_tile(int sprite) const
{
  return
  {
    (sprite % (sprite_surface_->width() / 16)) * 16,
    (sprite / (sprite_surface_->width() / 16)) * 16,
    16,
    16
  };
}
