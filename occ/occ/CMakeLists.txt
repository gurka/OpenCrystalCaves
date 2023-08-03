cmake_minimum_required(VERSION 3.2)

project(occ)

add_executable(occ
  "src/constants.h"
  "src/game_renderer.cc"
  "src/game_renderer.h"
  "src/occ.cc"
  "src/spritemgr.cc"
  "src/spritemgr.h"
)
target_link_libraries(occ
  "utils"
  "sdl_wrapper"
  "game"
  SDL2::SDL2
  SDL2_ttf::SDL2_ttf
)
# Linux apt doesn't have the latest SDL libs
if(UNIX)
target_link_libraries(occ ${SDL2TTF_LIBRARIES})
else()
target_link_libraries(occ SDL2_ttf::SDL2_ttf)
endif()

set_property(TARGET occ PROPERTY CXX_STANDARD 14)
