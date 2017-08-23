cmake_minimum_required(VERSION 3.0)

project(occ)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall -Wextra -Weffc++")

add_executable(occ
  "src/draw/line.cc"
  "src/draw/line.h"
  "src/draw/text.cc"
  "src/draw/text.h"

  "src/occ.cc"
  "src/config.h"
  "src/draw.h"
  "src/game.cc"
  "src/game.h"
  "src/geometry.cc"
  "src/geometry.h"
  "src/input.h"
  "src/item.cc"
  "src/item.h"
  "src/level_loader.cc"
  "src/level_loader.h"
  "src/level.cc"
  "src/level.h"
  "src/math.h"
  "src/spritemgr.cc"
  "src/spritemgr.h"
  "src/vector.h"
)
target_include_directories(occ SYSTEM PUBLIC
  "/usr/include/SDL2"
  "lib/json/src"
)
target_link_libraries(occ
  "SDL2"
  "SDL2_ttf"
)
