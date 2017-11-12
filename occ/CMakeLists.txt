cmake_minimum_required(VERSION 3.2)

project(occ)

# debug flags (-fuse-old=gold is for Travis)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=address -fsanitize=leak -fsanitize=undefined -fuse-ld=gold")

# sdl_wrapper
add_subdirectory("sdl_wrapper")
target_compile_options(sdl_wrapper PRIVATE -Wall -Wextra -Wpedantic -Weffc++)
target_include_directories(sdl_wrapper PUBLIC
  "utils/export"
)
target_include_directories(sdl_wrapper SYSTEM PUBLIC
  "/usr/include/SDL2"
)

# game
add_subdirectory("game")
target_compile_options(game PRIVATE -Wall -Wextra -Wpedantic -Weffc++)
target_include_directories(game PUBLIC
  "utils/export"
)
target_include_directories(game SYSTEM PUBLIC
  "external/json/src"
)

# utils
add_subdirectory("utils")
target_compile_options(utils PRIVATE -Wall -Wextra -Wpedantic -Weffc++)

# occ
add_subdirectory("occ")
target_compile_options(occ PRIVATE -Wall -Wextra -Wpedantic -Weffc++)
target_include_directories(occ PUBLIC
  "sdl_wrapper/export"
  "game/export"
  "utils/export"
)

# tests
enable_testing()
add_subdirectory(external/googletest)
add_test(game game/game_test)
add_test(sdl_wrapper sdl_wrapper/sdl_wrapper_test)
add_test(utils utils/utils_test)
