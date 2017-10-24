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
target_include_directories(sdl_wrapper PUBLIC
  "export"
)
set_property(TARGET sdl_wrapper PROPERTY CXX_STANDARD 14)

add_executable(sdl_wrapper_test
  "test/src/stubs/sdl_stub.cc"
  "test/src/sdl_wrapper_test.cc"
  "test/src/graphics_test.cc"
  "test/src/event_test.cc"
)
target_include_directories(sdl_wrapper_test PUBLIC
  "export"
  "test/src/stubs"
)
target_link_libraries(sdl_wrapper_test
  gtest_main
  gmock_main
  sdl_wrapper
  utils_stubs
)
set_property(TARGET sdl_wrapper_test PROPERTY CXX_STANDARD 14)
