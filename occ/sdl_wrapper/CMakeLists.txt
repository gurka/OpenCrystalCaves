cmake_minimum_required(VERSION 3.2)

project(sdl_wrapper)

add_library(sdl_wrapper
  "export/event.h"
  "export/graphics.h"
  "export/sdl_wrapper.h"
  "src/event_impl.cc"
  "src/event_impl.h"
  "src/graphics_impl.cc"
  "src/graphics_impl.h"
  "src/sdl_wrapper_impl.cc"
  "src/sdl_wrapper_impl.h"
)

set_property(TARGET sdl_wrapper PROPERTY CXX_STANDARD 14)

add_executable(sdl_wrapper_test "test/sdl_wrapper_test.cc")
