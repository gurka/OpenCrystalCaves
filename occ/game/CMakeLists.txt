cmake_minimum_required(VERSION 3.5)

project(game)

add_library(game
  "export/background.h"
  "export/game.h"
  "export/item.h"
  "export/level_id.h"
  "export/object.h"
  "export/player_input.h"
  "export/player.h"
  "src/game_impl.cc"
  "src/game_impl.h"
  "src/item_loader.cc"
  "src/item_loader.h"
  "src/item.cc"
  "src/level_loader.cc"
  "src/level_loader.h"
  "src/level.cc"
  "src/level.h"
  "src/moving_platform.h"
  "src/player.cc"
)

set_property(TARGET game PROPERTY CXX_STANDARD 14)
