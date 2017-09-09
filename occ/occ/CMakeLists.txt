cmake_minimum_required(VERSION 3.5)

project(occ)

add_executable(occ
  "src/input.h"
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
