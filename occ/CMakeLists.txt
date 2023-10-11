cmake_minimum_required(VERSION 3.15)

project(occ)

# debug flags (-fuse-old=gold is for Travis)
if(UNIX AND NOT APPLE)
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=address -fsanitize=leak -fsanitize=undefined -fuse-ld=gold")
endif()

find_package(SDL2 REQUIRED)

if(MSVC)
  set(COMPILE_OPTIONS -MP -W4 -WX -wd\"4090\" -wd\"4996\" -wd\"4204\" /D_SILENCE_TR1_NAMESPACE_DEPRECATION_WARNING)
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
  "external/json/include"
)

# utils
add_subdirectory("utils")
target_compile_options(utils PRIVATE ${COMPILE_OPTIONS})

# occ
add_subdirectory("occ")
target_compile_options(occ PRIVATE ${COMPILE_OPTIONS})
target_include_directories(occ PUBLIC
  "external/find_steam_game"
  "sdl_wrapper/export"
  "game/export"
  "utils/export"
)

add_subdirectory("tileset_reader")
target_compile_options(tileset_reader PRIVATE ${COMPILE_OPTIONS})
target_include_directories(tileset_reader SYSTEM PUBLIC
  "external/find_steam_game"
  "occ/occ/src"
  "sdl_wrapper/export"
  "utils/export"
)

# tests
enable_testing()
add_subdirectory(external/googletest)
if(MSVC)
  target_compile_options(gmock PRIVATE /D_SILENCE_TR1_NAMESPACE_DEPRECATION_WARNING)
  target_compile_options(gmock_main PRIVATE /D_SILENCE_TR1_NAMESPACE_DEPRECATION_WARNING)
  target_compile_options(gtest PRIVATE /D_SILENCE_TR1_NAMESPACE_DEPRECATION_WARNING)
  target_compile_options(gtest_main PRIVATE /D_SILENCE_TR1_NAMESPACE_DEPRECATION_WARNING)
endif()
# Match CRT used by googletest
if(MSVC)
  set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()
add_test(game game/game_test)
target_compile_options(game PRIVATE ${COMPILE_OPTIONS})
add_test(sdl_wrapper sdl_wrapper/sdl_wrapper_test)
target_compile_options(sdl_wrapper PRIVATE ${COMPILE_OPTIONS})
add_test(utils utils/utils_test)
target_compile_options(utils PRIVATE ${COMPILE_OPTIONS})
