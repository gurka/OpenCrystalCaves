add_executable(tileset_reader
  "tileset_reader.cc"
)
target_compile_features(tileset_reader PRIVATE cxx_std_17)
target_include_directories(tileset_reader PUBLIC
  ${SDL2_INCLUDE_DIRS}
)
target_link_libraries(occ
  SDL2::SDL2
)
