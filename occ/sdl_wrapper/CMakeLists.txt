cmake_minimum_required(VERSION 3.5)

project(sdl_wrapper)

add_library(sdl_wrapper
  "export/draw.h"
  "export/graphics.h"
  "export/line.h"
  "export/text.h"
  "src/graphics_impl.cc"
  "src/graphics_impl.h"
  "src/line.cc"
  "src/text.cc"
)

set_property(TARGET sdl_wrapper PROPERTY CXX_STANDARD 14)
