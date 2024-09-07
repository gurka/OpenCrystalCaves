add_executable(sprite_to_emoji
  "sprite_to_emoji.cc"
)
target_compile_features(sprite_to_emoji PRIVATE cxx_std_17)
target_link_libraries(sprite_to_emoji
  "utils"
)
