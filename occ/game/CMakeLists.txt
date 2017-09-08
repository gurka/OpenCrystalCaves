cmake_minimum_required(VERSION 3.5)

project(game)

add_library(game
  "export/game.h"
  "export/item.h"
  "export/level.h"
  "export/player_input.h"
  "export/player.h"
  "src/game_impl.cc"
  "src/game_impl.h"
  "src/item_loader.cc"
  "src/item_loader.h"
  "src/item.cc"
  "src/level_base.cc"
  "src/level_base.h"
  "src/level_loader.cc"
  "src/level_loader.h"
  "src/level_mainlevel.cc"
  "src/level_mainlevel.h"
  "src/level_one.cc"
  "src/level_one.h"
  "src/player.cc"
)

set_property(TARGET game PROPERTY CXX_STANDARD 14)
