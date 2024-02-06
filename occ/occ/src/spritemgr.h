#ifndef SPRITEMGR_H_
#define SPRITEMGR_H_

#include <memory>
#include <string>

#include "graphics.h"
#include "geometry.h"

// TODO: Rename files to sprite_manager.cc/h ?

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
	ICON_QUESTION_1 = 95,	// ?
	ICON_QUESTION_2 = 96,	// ? half
	ICON_QUESTION_3 = 97,	// ? half reverse
	ICON_QUESTION_4 = 98,	// !
	ICON_QUESTION_5 = 99,	// ? half (again)
	ICON_GUN = 100,
	ICON_HEART = 104,
	ICON_KEY = 105,
	ICON_0 = 108,
};

class SpriteManager
{
 public:
  SpriteManager()
    : sprite_surface_(), char_surface_()
  {
  }

  bool load_tilesets(Window& window, const int episode);
  const Surface* get_surface() const;
  geometry::Rectangle get_rect_for_tile(const int sprite) const;
	const Surface* get_char_surface() const;
  geometry::Rectangle get_rect_for_char(const wchar_t ch) const;
	Vector<int> render_text(const std::wstring& text, const Vector<int>& pos) const;
	geometry::Rectangle get_rect_for_number(const char ch) const;
	Vector<int> render_number(const int num, const Vector<int>& pos) const;
	geometry::Rectangle get_rect_for_icon(const int idx) const;
	void render_icon(const Icon icon, const Vector<int>& pos) const;

 private:
  std::unique_ptr<Surface> sprite_surface_;
	std::unique_ptr<Surface> char_surface_;
};

#endif  // SPRITEMGR_H_
