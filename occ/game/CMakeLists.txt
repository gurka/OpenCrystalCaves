cmake_minimum_required(VERSION 3.0)

project(game)

add_library(game
  "export/game.h"
  "export/item.h"
  "export/level.h"
  "export/player_input.h"
  "export/player.h"
  "src/game.cc"
  "src/item.cc"
  "src/level.cc"
)
