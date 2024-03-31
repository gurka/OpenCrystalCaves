#pragma once

#include <memory>
#include <string>

#include "geometry.h"
#include "graphics.h"

// TODO: Rename files to sprite_manager.cc/h ?
#define SPRITE_W 16
#define SPRITE_H 16

enum class Sprite : int
{
  SPRITE_CLEAR_BLOCK = 6,
  SPRITE_ENTRY_1 = 12,
  SPRITE_ENTRY_2 = 13,
  SPRITE_ENTRY_3 = 14,
  SPRITE_PISTOL = 308,
  SPRITE_POWER = 309,
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
  SPRITE_LIGHT_SWITCH_OFF = 429,
  SPRITE_LIGHT_SWITCH_ON = 430,
  SPRITE_LEVER_R_OFF = 432,
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
  SPRITE_MUSHROOM_RED = 668,
  SPRITE_MUSHROOM_GREEN = 669,
};

#define CHAR_W 8
#define CHAR_H 8

enum class Icon : int
{
  ICON_FRAME_NW = 0,
  ICON_FRAME_N = 1,
  ICON_FRAME_NE = 2,
  ICON_FRAME_SE = 3,
  ICON_FRAME_SW = 4,
  ICON_FRAME_W = 5,
  ICON_FRAME_S = 6,
  ICON_FRAME_E = 7,
  ICON_FRAME_INNER = 8,
  ICON_FRAME_SHADOW = 9,
  ICON_SPARKLE_1 = 37,
  ICON_SPARKLE_2 = 38,
  ICON_SPARKLE_3 = 39,
  ICON_SPARKLE_4 = 40,
  ICON_QUESTION_1 = 95,  // ?
  ICON_QUESTION_2 = 96,  // ? half
  ICON_QUESTION_3 = 97,  // ? half reverse
  ICON_QUESTION_4 = 98,  // !
  ICON_QUESTION_5 = 99,  // ? half (again)
  ICON_GUN = 100,
  ICON_HEART = 104,
  ICON_KEY = 105,
  ICON_0 = 108,
};

class SpriteManager
{
 public:
  SpriteManager() : sprite_surface_(), char_surface_() {}

  bool load_tilesets(Window& window, const int episode);
  const Surface* get_surface() const;
  geometry::Rectangle get_rect_for_tile(const int sprite) const;
  void render_tile(const int sprite, const geometry::Position& pos, const geometry::Position camera_position = {0, 0}) const;
  const Surface* get_char_surface() const;
  geometry::Rectangle get_rect_for_char(const wchar_t ch) const;
  geometry::Position render_text(const std::wstring& text, const geometry::Position& pos, const Color tint = {0xff, 0xff, 0xff}) const;
  geometry::Rectangle get_rect_for_number(const char ch) const;
  geometry::Position render_number(const int num, const geometry::Position& pos) const;
  geometry::Rectangle get_rect_for_icon(const int idx) const;
  void render_icon(const Icon icon, const geometry::Position& pos, const bool flip = false, const Color tint = {0xff, 0xff, 0xff}) const;

 private:
  std::unique_ptr<Surface> sprite_surface_;
  std::unique_ptr<Surface> char_surface_;
};
