cmake_minimum_required(VERSION 3.5)

project(occ)

add_executable(occ
  "src/constants.h"
  "src/game_renderer.cc"
  "src/game_renderer.h"
  "src/imagemgr.cc"
  "src/imagemgr.h"
  "src/occ.cc"
  "src/spritemgr.cc"
  "src/spritemgr.h"
  "src/state.cc"
  "src/state.h"
)
target_link_libraries(occ
  "utils"
  "sdl_wrapper"
  "game"
  SDL2::SDL2
  SDL2_image::SDL2_image
)

target_compile_features(occ PRIVATE cxx_std_17)
