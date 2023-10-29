add_executable(sound_player
  "sound_player.cc"
)
target_compile_features(sound_player PRIVATE cxx_std_17)
target_link_libraries(sound_player
  "utils"
  "sdl_wrapper"
  SDL2::SDL2
)
