cmake_minimum_required(VERSION 3.2)

project(utils)

add_library(utils
  "export/geometry.h"
  "export/logger.h"
  "export/occ_math.h"
  "export/misc.h"
  "export/vector.h"
  "src/geometry.cc"
  "src/logger.cc"
)

set_property(TARGET utils PROPERTY CXX_STANDARD 14)

add_executable(utils_test "test/utils_test.cc")
