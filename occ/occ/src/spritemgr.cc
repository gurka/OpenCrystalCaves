#include "spritemgr.h"

#include "config.h"

bool SpriteManager::load_tileset(const std::string& filename)
{
  // Check if tileset already loaded
  if (tileset_)
  {
    return false;
  }

  // Load tileset
  tileset_.reset(SDL_LoadBMP(filename.c_str()));
  if (!tileset_)
  {
    fprintf(stderr, "Could not load '%s': %s\n", filename.c_str(), SDL_GetError());
    return false;
  }
  SDL_SetColorKey(tileset_.get(), SDL_TRUE, SDL_MapRGB(tileset_->format, 0, 180, 0));

  return true;
}

int SpriteManager::number_of_tiles() const
{
  assert(tileset_);

  return (tileset_->w / 16) * (tileset_->h / 16);
}

SDL_Surface* SpriteManager::get_surface() const
{
  assert(tileset_);

  return tileset_.get();
}

SDL_Rect SpriteManager::get_rect_for_tile(int sprite) const
{
  assert(tileset_);
  assert(sprite >= 0);
  assert(sprite < number_of_tiles());

  return SDL_Rect
  {
    (sprite % (tileset_->w / 16)) * 16,
    (sprite / (tileset_->w / 16)) * 16,
    16,
    16
  };
}
