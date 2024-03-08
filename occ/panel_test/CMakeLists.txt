add_executable(panel_test
  "panel_test.cc"
  "../occ/src/panel.cc"
  "../occ/src/panel.h"
  "../occ/src/spritemgr.cc"
  "../occ/src/spritemgr.h"
)
target_compile_features(panel_test PRIVATE cxx_std_17)
target_link_libraries(panel_test
  "utils"
  "sdl_wrapper"
  "unlzexe"
  SDL2::SDL2
)
