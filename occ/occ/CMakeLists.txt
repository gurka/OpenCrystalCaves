cmake_minimum_required(VERSION 3.5)

project(occ)

file(GLOB AHEASING_HDRS "../external/AHEasing/AHEasing/*.h")
file(GLOB AHEASING_SRCS "../external/AHEasing/AHEasing/*.c")
add_executable(occ
  "src/constants.h"
  "src/game_renderer.cc"
  "src/game_renderer.h"
  "src/imagemgr.cc"
  "src/imagemgr.h"
  "src/occ.cc"
  "src/panel.cc"
  "src/panel.h"
  "src/spritemgr.cc"
  "src/spritemgr.h"
  "src/state.cc"
  "src/state.h"
  "src/utils.cc"
  "src/utils.h"
  "src/wchars.h"
  ${AHEASING_HDRS} ${AHEASING_SRCS}
)
target_include_directories(occ PUBLIC
  "../external/AHEasing/AHEasing"
  "../game/src"
)
target_compile_definitions(occ PRIVATE AH_EASING_USE_DBL_PRECIS _USE_MATH_DEFINES)
target_link_libraries(occ
  "utils"
  "sdl_wrapper"
  "game"
  SDL2::SDL2
  SDL2_image::SDL2_image
)
if(APPLE)
	set_target_properties(occ PROPERTIES
		MACOSX_RPATH 1
		BUILD_WITH_INSTALL_RPATH 1
		INSTALL_RPATH "@loader_path/../Frameworks;/Library/Frameworks")
endif()

target_compile_features(occ PRIVATE cxx_std_17)
