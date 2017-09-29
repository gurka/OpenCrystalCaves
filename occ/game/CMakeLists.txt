cmake_minimum_required(VERSION 3.5)

project(game)

add_library(game
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

set_property(TARGET game PROPERTY CXX_STANDARD 14)
