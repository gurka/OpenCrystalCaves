add_executable(sprite_viewer
  "sprite_viewer.cc"
  "../occ/src/spritemgr.cc"
  "../occ/src/spritemgr.h"
)
target_compile_features(sprite_viewer PRIVATE cxx_std_17)
target_link_libraries(sprite_viewer
  "utils"
  "sdl_wrapper"
  SDL2::SDL2
)
