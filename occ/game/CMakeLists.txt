cmake_minimum_required(VERSION 3.0)

project(game)

add_library(game
  "export/item.h"
  "export/level.h"
  "src/item.cc"
  "src/level.cc"
)
