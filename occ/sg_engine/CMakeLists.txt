cmake_minimum_required(VERSION 3.2)

project(sg_engine)

add_library(sg_engine
  "export/event.h"
  "export/graphics.h"
  "export/sg_engine.h"
  "src/event_impl.cc"
  "src/event_impl.h"
  "src/graphics_impl.cc"
  "src/graphics_impl.h"
  "src/sg_engine_impl.cc"
  "src/sg_engine_impl.h"
)
target_include_directories(sg_engine PUBLIC
  "export"
)
set_property(TARGET sg_engine PROPERTY CXX_STANDARD 14)

#add_executable(sg_engine_test
#  "test/src/stubs/sdl_stub.cc"
#  "test/src/sg_engine_test.cc"
#  "test/src/graphics_test.cc"
#  "test/src/event_test.cc"
#)
#target_include_directories(sg_engine_test PUBLIC
#  "export"
#  "test/src/stubs"
#)
#target_link_libraries(sg_engine_test
#  gtest_main
#  gmock_main
#  sg_engine
#  utils_stubs
#)
#set_property(TARGET sg_engine_test PROPERTY CXX_STANDARD 14)
