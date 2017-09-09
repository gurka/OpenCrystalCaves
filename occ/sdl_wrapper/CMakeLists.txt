cmake_minimum_required(VERSION 3.5)

project(sdl_wrapper)

add_library(sdl_wrapper
  "export/draw.h"
  "export/line.h"
  "export/sdl_wrapper.h"
  "export/text.h"
  "src/line.cc"
  "src/sdl_wrapper_impl.cc"
  "src/sdl_wrapper_impl.h"
  "src/text.cc"
)

set_property(TARGET sdl_wrapper PROPERTY CXX_STANDARD 14)
