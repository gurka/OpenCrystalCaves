add_executable(ui_test
  "ui_test.cc"
  "../occ/src/spritemgr.cc"
  "../occ/src/spritemgr.h"
)
target_compile_features(ui_test PRIVATE cxx_std_17)
target_link_libraries(ui_test
  "utils"
  "sdl_wrapper"
  SDL2::SDL2
)
