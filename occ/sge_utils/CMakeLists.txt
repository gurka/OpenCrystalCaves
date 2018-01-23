cmake_minimum_required(VERSION 3.2)

project(sge_utils)

add_library(sge_utils
  "export/geometry.h"
  "export/logger.h"
  "export/occ_math.h"
  "export/misc.h"
  "export/vector.h"
  "src/logger.cc"
)
target_include_directories(sge_utils PUBLIC
  "export"
)
set_property(TARGET sge_utils PROPERTY CXX_STANDARD 14)

add_executable(sge_utils_test
  "test/src/geometry_test.cc"
  "test/src/misc_test.cc"
  "test/src/occ_math_test.cc"
  "test/src/vector_test.cc"
)
target_include_directories(sge_utils_test PUBLIC
  "export"
)
target_link_libraries(sge_utils_test
  gtest_main
  gmock_main
  sge_utils
)
set_property(TARGET sge_utils_test PROPERTY CXX_STANDARD 14)

add_library(sge_utils_stubs
	"test/stubs/logger_stub.cc"
)
target_include_directories(sge_utils_stubs PUBLIC
  "export"
)
set_property(TARGET sge_utils_stubs PROPERTY CXX_STANDARD 14)
