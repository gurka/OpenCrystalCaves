cmake_minimum_required(VERSION 3.2)

project(occ)

# debug flags (-fuse-old=gold is for Travis)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=address -fsanitize=leak -fsanitize=undefined -fuse-ld=gold")

# sge
add_subdirectory("sge")
target_compile_options(sge PRIVATE -Wall -Wextra -Wpedantic -Weffc++)
target_include_directories(sge PUBLIC
  "sge_utils/export"
)
target_include_directories(sge SYSTEM PUBLIC
  "/usr/include/SDL2"
)

# game
add_subdirectory("game")
target_compile_options(game PRIVATE -Wall -Wextra -Wpedantic -Weffc++)
target_include_directories(game PUBLIC
  "sge_utils/export"
)
target_include_directories(game SYSTEM PUBLIC
  "external/json/src"
)

# sge_utils
add_subdirectory("sge_utils")
target_compile_options(sge_utils PRIVATE -Wall -Wextra -Wpedantic -Weffc++)

# occ
add_subdirectory("occ")
target_compile_options(occ PRIVATE -Wall -Wextra -Wpedantic -Weffc++)
target_include_directories(occ PUBLIC
  "sge/export"
  "game/export"
  "sge_utils/export"
)

# tests
#enable_testing()
#add_subdirectory(external/googletest)
#add_test(game game/game_test)
#add_test(sge sge/sge_test)
#add_test(sge_utils sge_utils/sge_utils_test)
