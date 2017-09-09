cmake_minimum_required(VERSION 3.5)

project(occ)

add_compile_options(-Wall -Wextra -Wpedantic -Weffc++)

# sdl_wrapper
add_subdirectory("sdl_wrapper")
target_include_directories(sdl_wrapper PUBLIC
  "sdl_wrapper/export"
  "utils/export"
)
target_include_directories(sdl_wrapper SYSTEM PUBLIC
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
  "sdl_wrapper/export"
  "game/export"
  "utils/export"
)
target_include_directories(occ SYSTEM PUBLIC
  "/usr/include/SDL2"
)
