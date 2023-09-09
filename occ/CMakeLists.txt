cmake_minimum_required(VERSION 3.2)

project(occ)

# debug flags (-fuse-old=gold is for Travis)
if(UNIX AND NOT APPLE)
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=address -fsanitize=leak -fsanitize=undefined -fuse-ld=gold")
endif()

find_package(SDL2 REQUIRED)

if(MSVC)
  set(COMPILE_OPTIONS -MP -W4 -WX -wd\"4090\" -wd\"4996\" -wd\"4204\")
else()
  set(COMPILE_OPTIONS -Wall -Wextra -Wpedantic -Weffc++)
endif()

# sdl_wrapper
add_subdirectory("sdl_wrapper")
target_compile_options(sdl_wrapper PRIVATE ${COMPILE_OPTIONS})
target_include_directories(sdl_wrapper PUBLIC
  "utils/export"
)

# game
add_subdirectory("game")
target_compile_options(game PRIVATE ${COMPILE_OPTIONS})
target_include_directories(game PUBLIC
  "utils/export"
)
target_include_directories(game SYSTEM PUBLIC
  "external/json/src"
)

# utils
add_subdirectory("utils")
target_compile_options(utils PRIVATE ${COMPILE_OPTIONS})

# occ
add_subdirectory("occ")
target_compile_options(occ PRIVATE ${COMPILE_OPTIONS})
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
