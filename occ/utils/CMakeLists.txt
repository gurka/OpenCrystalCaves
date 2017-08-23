cmake_minimum_required(VERSION 3.0)

project(utils)

add_library(utils
  "export/geometry.h"
  "export/logger.h"
  "export/math.h"
  "export/vector.h"
  "src/geometry.cc"
  "src/logger.cc"
)
