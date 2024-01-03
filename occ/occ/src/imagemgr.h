#pragma once

#include <array>
#include <memory>
#include <vector>

#include "geometry.h"
#include "graphics.h"

enum CCImage
{
  IMAGE_APOGEE = 0,   // ext .APG
  IMAGE_TITLE = 1,    // ext .TTL
  IMAGE_CREDITS = 2,  // ext .CDT
  IMAGE_END = 3,      // ext .END
};

class ImageManager
{
 public:
  ImageManager() : episode_images_() {}

  bool load_images(Window& window);
  size_t number_of_episodes() const { return episode_images_.size(); }
  std::vector<Surface*> get_images(size_t episode, CCImage image) const;

 private:
  // Episode, image type, images
  std::vector<std::array<std::vector<std::unique_ptr<Surface>>, 4>> episode_images_;
};
