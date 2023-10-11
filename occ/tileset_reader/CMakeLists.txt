add_executable(tileset_reader
  "tileset_reader.cc"
  "../occ/src/spritemgr.cc"
  "../occ/src/spritemgr.h"
)
target_compile_features(tileset_reader PRIVATE cxx_std_17)
target_link_libraries(tileset_reader
  "utils"
  "sdl_wrapper"
  SDL2::SDL2
)
