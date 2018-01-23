cmake_minimum_required(VERSION 3.2)

project(sge)

add_library(sge
  "export/input.h"
  "export/graphics.h"
  "export/sge.h"
  "src/graphics_impl.cc"
  "src/graphics_impl.h"
  "src/sge_impl.cc"
  "src/sge_impl.h"
)
target_include_directories(sge PUBLIC
  "export"
)
set_property(TARGET sge PROPERTY CXX_STANDARD 14)

#add_executable(sge_test
#  "test/src/stubs/sdl_stub.cc"
#  "test/src/sge_test.cc"
#  "test/src/graphics_test.cc"
#  "test/src/event_test.cc"
#)
#target_include_directories(sge_test PUBLIC
#  "export"
#  "test/src/stubs"
#)
#target_link_libraries(sge_test
#  gtest_main
#  gmock_main
#  sge
#  utils_stubs
#)
#set_property(TARGET sge_test PROPERTY CXX_STANDARD 14)
