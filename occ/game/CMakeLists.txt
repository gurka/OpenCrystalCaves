cmake_minimum_required(VERSION 3.2)

project(game)

add_library(game
  "export/background.h"
  "export/enemy.h"
  "export/game.h"
  "export/item.h"
  "export/level_id.h"
  "export/object.h"
  "export/player_input.h"
  "export/player.h"
  "export/tile.h"
  "src/background.cc"
  "src/enemy.cc"
  "src/explosion.cc"
  "src/explosion.h"
  "src/game_impl.cc"
  "src/game_impl.h"
  "src/item.cc"
  "src/level_loader.cc"
  "src/level_loader.h"
  "src/level.h"
  "src/missile.cc"
  "src/missile.h"
  "src/moving_platform.h"
  "src/object_manager.cc"
  "src/object_manager.h"
  "src/player.cc"
  "src/tile.cc"
)
target_include_directories(game PUBLIC
  "export"
)
set_property(TARGET game PROPERTY CXX_STANDARD 14)

add_executable(game_test
  "test/src/game_test.cc"
)
target_include_directories(game_test PUBLIC
  "export"
)
target_link_libraries(game_test
  gtest_main
  gmock_main
  game
)
set_property(TARGET game_test PROPERTY CXX_STANDARD 14)
