cmake_minimum_required(VERSION 3.0)

project(occ)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall -Wextra -Weffc++")

# gfx
add_subdirectory("gfx")
target_include_directories(gfx PUBLIC
  "gfx/export"
  "utils/export"
)
target_include_directories(gfx SYSTEM PUBLIC
  "/usr/include/SDL2"
)

# game
add_subdirectory("game")
target_include_directories(game PUBLIC
  "game/export"
  "utils/export"
)

# io
add_subdirectory("io")
target_include_directories(io PUBLIC
  "io/export"
  "utils/export"
  "game/export"
)
target_include_directories(io SYSTEM PUBLIC
  "external/json/src"
)

# utils
add_subdirectory("utils")
target_include_directories(utils PUBLIC
  "utils/export"
)

# occ
add_subdirectory("occ")
target_include_directories(occ PUBLIC
  "gfx/export"
  "game/export"
  "io/export"
  "utils/export"
)
target_include_directories(occ SYSTEM PUBLIC
  "/usr/include/SDL2"
)
