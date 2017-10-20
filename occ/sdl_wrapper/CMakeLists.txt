cmake_minimum_required(VERSION 3.2)

project(sdl_wrapper)

set(sdl_wrapper_src
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


add_library(sdl_wrapper ${sdl_wrapper_src})
target_include_directories(sdl_wrapper PUBLIC
  "export"
)

set_property(TARGET sdl_wrapper PROPERTY CXX_STANDARD 14)

add_executable(sdl_wrapper_test
#  ${sdl_wrapper_src}
  "test/src/sdl_wrapper_test.cc"
)
target_include_directories(sdl_wrapper_test PUBLIC
  "export"
)
