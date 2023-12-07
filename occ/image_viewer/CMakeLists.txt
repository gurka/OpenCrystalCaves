add_executable(image_viewer
  "image_viewer.cc"
  "../occ/src/imagemgr.cc"
  "../occ/src/imagemgr.h"
)
target_compile_features(image_viewer PRIVATE cxx_std_17)
target_link_libraries(image_viewer
  "utils"
  "sdl_wrapper"
  SDL2::SDL2
  SDL2_image::SDL2_image
)
