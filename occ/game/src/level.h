#pragma once

#include <vector>

#include "geometry.h"
#include "level_id.h"
#include "moving_platform.h"
#include "object_manager.h"
#include "tile.h"

enum class Sprite : int
{
  SPRITE_CLEAR_BLOCK = 6,
  SPRITE_ENTRY_1 = 12,
  SPRITE_ENTRY_2 = 13,
  SPRITE_ENTRY_3 = 14,
  SPRITE_ROCKMAN_L_1 = 104,
  SPRITE_ROCKMAN_L_2 = 105,
  SPRITE_ROCKMAN_L_3 = 106,
  SPRITE_ROCKMAN_L_4 = 107,
  SPRITE_ROCKMAN_L_5 = 108,
  SPRITE_ROCKMAN_WALK_L_1 = 109,
  SPRITE_ROCKMAN_WALK_L_2 = 110,
  SPRITE_ROCKMAN_WALK_L_3 = 111,
  SPRITE_ROCKMAN_WALK_L_4 = 112,
  SPRITE_ROCKMAN_WALK_L_5 = 113,
  SPRITE_ROCKMAN_WALK_L_6 = 114,
  SPRITE_ROCKMAN_WALK_L_7 = 115,
  SPRITE_HORIZON_LAMP = 240,
  SPRITE_HORIZON_1 = 247,
  SPRITE_HORIZON_2 = 248,
  SPRITE_HORIZON_3 = 249,
  SPRITE_HORIZON_4 = 250,
  SPRITE_MINE_SIGN_1 = 251,
  SPRITE_MINE_SIGN_2 = 252,
  SPRITE_STANDING_RIGHT = 260,
  SPRITE_PISTOL = 308,
  SPRITE_POWER = 309,
  SPRITE_STARS = 350,  // 4x2
  SPRITE_LIGHT_SWITCH_OFF = 429,
  SPRITE_LIGHT_SWITCH_ON = 430,
  SPRITE_LEVER_R_OFF = 432,
  SPRITE_DANGER_1 = 476,
  SPRITE_DANGER_2 = 477,
  SPRITE_EXIT_TOP_LEFT_1 = 584,
  SPRITE_EXIT_TOP_LEFT_2 = 585,
  SPRITE_EXIT_TOP_LEFT_3 = 586,
  SPRITE_EXIT_TOP_LEFT_4 = 587,
  SPRITE_EXIT_BOTTOM_LEFT_1 = 588,
  SPRITE_EXIT_BOTTOM_LEFT_2 = 589,
  SPRITE_EXIT_BOTTOM_LEFT_3 = 590,
  SPRITE_EXIT_BOTTOM_LEFT_4 = 591,
  SPRITE_EXIT_TOP_RIGHT_1 = 592,
  SPRITE_EXIT_BOTTOM_RIGHT_1 = 596,
  SPRITE_EXIT_BOTTOM_RIGHT_2 = 597,
  SPRITE_EXIT_BOTTOM_RIGHT_3 = 598,
  SPRITE_EXIT_BOTTOM_RIGHT_4 = 599,
  SPRITE_SWITCH_OFF = 602,
  SPRITE_SWITCH_ON = 603,
  SPRITE_GRAVITY = 609,
  SPRITE_WOOD_H = 620,
  SPRITE_CRYSTAL_1_R = 624,
  SPRITE_CRYSTAL_1_Y = 625,
  SPRITE_CRYSTAL_1_G = 626,
  SPRITE_CRYSTAL_1_B = 627,
  SPRITE_EARTH = 632,
  SPRITE_MOON = 634,
  SPRITE_CRYSTAL_2_R = 636,
  SPRITE_CRYSTAL_2_Y = 637,
  SPRITE_CRYSTAL_2_G = 638,
  SPRITE_CRYSTAL_2_B = 639,
  SPRITE_CRYSTAL_3_R = 656,
  SPRITE_CRYSTAL_3_Y = 657,
  SPRITE_CRYSTAL_3_G = 658,
  SPRITE_CRYSTAL_3_B = 659,
  SPRITE_CRYSTAL_HIDDEN = 660,
  SPRITE_TORCH_1 = 663,
  SPRITE_TORCH_2 = 664,
  SPRITE_TORCH_3 = 665,
  SPRITE_TORCH_4 = 666,
  SPRITE_MUSHROOM_RED = 668,
  SPRITE_MUSHROOM_GREEN = 669,
  SPRITE_VOLCANO_TOP_1 = 737,
  SPRITE_VOLCANO_TOP_2 = 738,
  SPRITE_VOLCANO_BOTTOM_1 = 740,
  SPRITE_VOLCANO_BOTTOM_2 = 741,
  SPRITE_VOLCANO_BOTTOM_3 = 742,
  SPRITE_VOLCANO_BOTTOM_4 = 743,
  SPRITE_CONCRETE = 1092,
  SPRITE_CONCRETE_H = 1093,
  SPRITE_CONCRETE_V = 1094,
  SPRITE_CONCRETE_X = 1095,
};

struct Level
{
  Level(LevelId level_id,
        int width,
        int height,
        const geometry::Position& player_spawn,
        const std::string& background,
        const std::vector<int>& tile_ids,
        std::vector<int>&& item_ids,
        std::vector<MovingPlatform>&& moving_platforms,
        const ObjectManager& object_manager,
        const bool decode);

  LevelId level_id;

  int width;
  int height;

  geometry::Position player_spawn;

  const Tile& get_tile(const int x, const int y) const;

  std::string background;
  std::vector<Tile> tiles;
  std::vector<int> item_ids;

  std::vector<MovingPlatform> moving_platforms;
};
