cmake_minimum_required(VERSION 3.5)

project(gfx)

add_library(gfx
  "export/draw.h"
  "export/line.h"
  "export/text.h"
  "src/line.cc"
  "src/text.cc"
)

set_property(TARGET gfx PROPERTY CXX_STANDARD 14)
