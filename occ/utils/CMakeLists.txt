cmake_minimum_required(VERSION 3.5)

project(utils)

add_library(utils
  "export/geometry.h"
  "export/logger.h"
  "export/occ_math.h"
  "export/misc.h"
  "export/path.h"
  "export/vector.h"
  "src/logger.cc"
  "src/path.cc"
)
target_include_directories(utils PUBLIC
  "export"
  "../external/find_steam_game"
)
target_compile_features(utils PRIVATE cxx_std_17)

add_executable(utils_test
  "test/src/geometry_test.cc"
  "test/src/misc_test.cc"
  "test/src/occ_math_test.cc"
  "test/src/vector_test.cc"
)
target_include_directories(utils_test PUBLIC
  "export"
)
target_link_libraries(utils_test
  gtest_main
  gmock_main
  utils
)
target_compile_features(utils_test PRIVATE cxx_std_17)

add_library(utils_stubs
	"test/stubs/logger_stub.cc"
)
target_include_directories(utils_stubs PUBLIC
  "export"
)
target_compile_features(utils_stubs PRIVATE cxx_std_17)
