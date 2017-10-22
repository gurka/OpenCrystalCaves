cmake_minimum_required(VERSION 3.2)

project(occ)

add_executable(occ
  "src/constants.h"
  "src/game_renderer.cc"
  "src/game_renderer.h"
  "src/occ.cc"
  "src/spritemgr.cc"
  "src/spritemgr.h"
)
target_link_libraries(occ
  "utils"
  "sdl_wrapper"
  "game"
  "SDL2"
  "SDL2_ttf"
)
set_property(TARGET occ PROPERTY CXX_STANDARD 14)
