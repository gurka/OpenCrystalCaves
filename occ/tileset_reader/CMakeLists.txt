add_executable(tileset_reader
  "tileset_reader.cc"
)
set_property(TARGET tileset_reader PROPERTY CXX_STANDARD 14)
target_include_directories(tileset_reader PUBLIC
  ${SDL2_INCLUDE_DIRS}
)
target_link_libraries(occ
  SDL2::SDL2
)
