cmake_minimum_required(VERSION 3.0)

project(io)

add_library(io
  "export/item_loader.h"
  "export/level_loader.h"
  "src/item_loader.cc"
  "src/level_loader.cc"
)
