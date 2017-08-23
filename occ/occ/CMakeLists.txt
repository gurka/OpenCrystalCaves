cmake_minimum_required(VERSION 3.0)

project(occ)

add_executable(occ
  "src/config.h"
  "src/game.cc"
  "src/game.h"
  "src/input.h"
  "src/occ.cc"
  "src/spritemgr.cc"
  "src/spritemgr.h"
)
target_link_libraries(occ
  "game"
  "gfx"
  "io"
  "utils"
  "SDL2"
  "SDL2_ttf"
)
