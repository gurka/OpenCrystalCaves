add_executable(font_viewer
  "font_viewer.cc"
  "../occ/src/spritemgr.cc"
  "../occ/src/spritemgr.h"
)
target_compile_features(font_viewer PRIVATE cxx_std_17)
target_link_libraries(font_viewer
  "utils"
  "sdl_wrapper"
  SDL2::SDL2
)
