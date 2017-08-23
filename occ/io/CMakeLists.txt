cmake_minimum_required(VERSION 3.0)

project(io)

add_library(io
  "export/level_loader.h"
  "src/level_loader.cc"
)
