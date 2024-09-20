cmake_minimum_required(VERSION 3.5)

project(game)

add_library(game
  "export/actor.h"
  "export/enemy.h"
  "export/entrance.h"
  "export/game.h"
  "export/hazard.h"
  "export/item.h"
  "export/level_id.h"
  "export/object.h"
  "export/player_input.h"
  "export/player.h"
  "export/tile.h"
  "src/actor.cc"
  "src/enemy.cc"
  "src/entrance.cc"
  "src/explosion.cc"
  "src/explosion.h"
  "src/game_impl.cc"
  "src/game_impl.h"
  "src/hazard.cc"
  "src/item.cc"
  "src/level_loader.cc"
  "src/level_loader.h"
  "src/level.h"
  "src/level.cc"
  "src/missile.cc"
  "src/missile.h"
  "src/moving_platform.h"
  "src/moving_platform.cc"
  "src/player.cc"
  "src/tile.cc"
)
target_link_libraries(game
  "utils"
)
target_include_directories(game PUBLIC
  "export"
)
target_compile_features(game PRIVATE cxx_std_17)

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
target_compile_features(game_test PRIVATE cxx_std_17)
