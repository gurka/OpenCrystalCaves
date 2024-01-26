add_executable(char_viewer
  "char_viewer.cc"
  "../occ/src/spritemgr.cc"
  "../occ/src/spritemgr.h"
)
target_compile_features(char_viewer PRIVATE cxx_std_17)
target_link_libraries(char_viewer
  "utils"
  "sdl_wrapper"
  SDL2::SDL2
)
