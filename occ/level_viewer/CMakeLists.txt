add_executable(level_viewer
  "level_viewer.cc"
  "../occ/src/spritemgr.cc"
  "../occ/src/spritemgr.h"
  "../occ/src/utils.cc"
  "../occ/src/utils.h"
)
target_compile_features(level_viewer PRIVATE cxx_std_17)
target_link_libraries(level_viewer
  "game"
  "utils"
  "sdl_wrapper"
  "unlzexe"
  SDL2::SDL2
)
