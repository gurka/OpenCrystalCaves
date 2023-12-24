cmake_minimum_required(VERSION 3.5)

project(sdl_wrapper)

include_directories(${SDL2_INCLUDE_DIRS})

file(GLOB SDLBITMAPFONT_HDRS "../external/SDLBitmapFont/*.h")
file(GLOB SDLBITMAPFONT_SRCS "../external/SDLBitmapFont/*.cpp")
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
  ${SDLBITMAPFONT_HDRS} ${SDLBITMAPFONT_SRCS}
)
target_include_directories(sdl_wrapper PUBLIC
  "export"
  ${SDL2_INCLUDE_DIRS}
  "../external/SDLBitmapFont"
)
target_link_libraries(sdl_wrapper SDL2::SDL2 SDL2_image::SDL2_image)
target_compile_features(sdl_wrapper PRIVATE cxx_std_17)

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
target_compile_features(sdl_wrapper_test PRIVATE cxx_std_17)
