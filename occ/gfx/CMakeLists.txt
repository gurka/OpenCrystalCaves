cmake_minimum_required(VERSION 3.5)

project(gfx)

add_library(gfx
  "export/draw.h"
  "export/line.h"
  "export/sdl_wrapper.h"
  "export/text.h"
  "src/line.cc"
  "src/sdl_wrapper_impl.cc"
  "src/sdl_wrapper_impl.h"
  "src/text.cc"
)

set_property(TARGET gfx PROPERTY CXX_STANDARD 14)
