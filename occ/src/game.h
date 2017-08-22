#ifndef GAME_H_
#define GAME_H_

#include <array>
#include <memory>

#include "SDL.h"

#include "config.h"
#include "geometry.h"
#include "input.h"
#include "item.h"
#include "spritemgr.h"
#include "vector.h"
#include "level.h"
#include "level_loader.h"

class Game
{
 public:
  Game();

  bool init(const SDL_Surface* windowSurface);

  void update(const Input& input);
  void render(SDL_Surface* dest) const;

 private:

  struct Player
  {
    geometry::Position position = geometry::Position();
    Vector<int> velocity = Vector<int>();

    enum class Direction
    {
      right,
      left
    } direction = Direction::right;

    enum class State
    {
      still,
      walking,
      jumping
    } state = State::still;

    unsigned animation_tick = 0u;
    unsigned jump_tick = 0u;

    static constexpr int sprite_standing_right = 244;
    static constexpr std::array<int, 12> sprite_walking_right =
    {
        244, 245, 246, 247, 248, 249, 250, 251, 252, 253, 254, 255
    };
    static constexpr int sprite_jumping_right = 268;

    static constexpr int sprite_standing_left = 256;
    static constexpr std::array<int, 12> sprite_walking_left =
    {
        256, 257, 258, 259, 260, 261, 262, 263, 264, 265, 266, 267
    };
    static constexpr int sprite_jumping_left = 269;
  } player_;

  bool initialized_;
  std::unique_ptr<SDL_Surface, decltype(&SDL_FreeSurface)> surface_game_;
  SpriteManager sprite_manager_;
  ItemManager item_manager_;
  Level level_;
  LevelLoader level_loader_;
  unsigned update_tick_;

  // debug stuff
  bool draw_debug_;
  bool draw_aabbs_;
  bool collide_x_;
  bool collide_y_;
  bool input_left_;
  bool input_right_;
  bool input_jump_;
};

#endif  // GAME_H_
