cmake_minimum_required(VERSION 3.2)

project(game)

set(game_src
  "export/game.h"
  "export/level_id.h"
  "export/object.h"
  "export/player_input.h"
  "export/player.h"
  "src/game_impl.cc"
  "src/game_impl.h"
  "src/level_loader.cc"
  "src/level_loader.h"
  "src/level.h"
  "src/moving_platform.h"
  "src/object_manager.cc"
  "src/object_manager.h"
  "src/player.cc"
)

add_library(game ${game_src})
target_include_directories(game PUBLIC
  "export"
)

set_property(TARGET game PROPERTY CXX_STANDARD 14)

add_executable(game_test
#  ${game_src}
  "test/src/game_test.cc"
)
target_include_directories(game_test PUBLIC
  "export"
)
