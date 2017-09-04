cmake_minimum_required(VERSION 3.5)

project(occ)

add_compile_options(-Wall -Wextra -Wpedantic -Weffc++)

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
target_include_directories(game SYSTEM PUBLIC
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
  "utils/export"
)
target_include_directories(occ SYSTEM PUBLIC
  "/usr/include/SDL2"
)
