#include "panel.h"

#include <codecvt>

#include "constants.h"
#include "misc.h"

constexpr Icon S_ICONS[]{Icon::ICON_SPARKLE_1, Icon::ICON_SPARKLE_2, Icon::ICON_SPARKLE_3, Icon::ICON_SPARKLE_4};

// https://stackoverflow.com/a/42844629/2038264
#if __cplusplus >= 201703L  // C++17 and later
#include <string_view>

static bool ends_with(std::string_view str, std::string_view suffix)
{
  return str.size() >= suffix.size() && str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
}

static bool starts_with(std::string_view str, std::string_view prefix)
{
  return str.size() >= prefix.size() && str.compare(0, prefix.size(), prefix) == 0;
}
#endif  // C++17


Panel::Panel(const char* ucsd)
{
  // Convert input text into a vector of strings, ignoring the
  // END OF WINDOW text
  // Also find the max string size to determine window size
  std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
  constexpr size_t len_limit = 33;
  size_t max_len = 0;
  const char* p_ucsd = ucsd;
  while (true)
  {
    const size_t len = *p_ucsd;
    max_len = std::max(len, max_len);
    p_ucsd++;
    std::string s(p_ucsd, len);
    if (len == 85 || s == "END OF WINDOW")
    {
      break;
    }
    // Add a paragraph break if the line is longer than the limit
    if (len > len_limit)
    {
      strings_.push_back(L"");
    }
    // If too long, break the string
    while (s.size() > len_limit)
    {
      strings_.push_back(converter.from_bytes(s.substr(0, len_limit)));
      s = s.substr(len_limit);
    }
    // Find the spinning question mark
    const auto question = s.find_first_of("^");
    if (question != std::string::npos)
    {
      question_pos_ = geometry::Position((static_cast<int>(question) + 2) * CHAR_W, (static_cast<int>(strings_.size()) + 2) * CHAR_H);
    }
    strings_.push_back(converter.from_bytes(s));
    // Add a paragraph break after some special lines
    if (ends_with(s, "-") || ends_with(s, ": ") || ends_with(s, "."))
    {
      strings_.push_back(L"");
    }
    // End immediately on some special lines
    if (ends_with(s, "^"))
    {
      break;
    }
    p_ucsd += len;
  }
  max_len = std::min(max_len, len_limit);
  size_ = geometry::Position(static_cast<int>(max_len) + 1, static_cast<int>(strings_.size()) + 1);
}

void Panel::update()
{
  ticks_ += 1;

  // Randomly switch the sparkle position around
  const auto sparkle_frame = (ticks_ / 3) % (std::size(S_ICONS) + 1);
  if (sparkle_frame == 0)
  {
    if (misc::random<int>(0, 1) == 0)
    {
      // Put the sparkle on the top edge
      sparkle_pos_ = geometry::Position(misc::random<int>(1, size_.x()) * CHAR_W, 0);
    }
    else
    {
      // Put the sparkle on the left edge
      sparkle_pos_ = geometry::Position(0, misc::random<int>(1, size_.y()) * CHAR_H);
    }
  }
}

void Panel::draw(const SpriteManager& sprite_manager) const
{
  const geometry::Position frame_pos(((SCREEN_SIZE.x() / CHAR_W - size_.x() - 1) / 2 - 1) * CHAR_W,
                                     ((SCREEN_SIZE.y() / CHAR_H - size_.y() - 1) / 2 - 1) * CHAR_H);
  const geometry::Size frame_size = size_ + geometry::Size(2, 2);
  // Draw frame
  // Top-left corner
  sprite_manager.render_icon(Icon::ICON_FRAME_NW, frame_pos);
  // Top edge
  for (int x = 1; x < frame_size.x(); x++)
  {
    sprite_manager.render_icon(Icon::ICON_FRAME_N, frame_pos + geometry::Position(x * CHAR_W, 0));
  }
  // Top-right corner
  sprite_manager.render_icon(Icon::ICON_FRAME_NE, frame_pos + geometry::Position(frame_size.x() * CHAR_W, 0));
  // Left edge
  for (int y = 1; y < frame_size.y(); y++)
  {
    sprite_manager.render_icon(Icon::ICON_FRAME_W, frame_pos + geometry::Position(0, y * CHAR_H));
  }
  // Right edge
  for (int y = 1; y < frame_size.y(); y++)
  {
    sprite_manager.render_icon(Icon::ICON_FRAME_E, frame_pos + geometry::Position(frame_size.x() * CHAR_W, y * CHAR_W));
  }
  // Bottom-left corner
  sprite_manager.render_icon(Icon::ICON_FRAME_SW, frame_pos + geometry::Position(0, frame_size.y() * CHAR_H));
  // Bottom edge
  for (int x = 1; x < frame_size.x(); x++)
  {
    sprite_manager.render_icon(Icon::ICON_FRAME_S, frame_pos + geometry::Position(x * CHAR_W, frame_size.y() * CHAR_H));
  }
  // Bottom-right corner
  sprite_manager.render_icon(Icon::ICON_FRAME_SE, frame_pos + geometry::Position(frame_size.x() * CHAR_W, frame_size.y() * CHAR_H));

  // Frame interior
  for (int y = 1; y < frame_size.y(); y++)
  {
    for (int x = 1; x < frame_size.x(); x++)
    {
      sprite_manager.render_icon(Icon::ICON_FRAME_INNER, frame_pos + geometry::Position(x * CHAR_W, y * CHAR_H));
    }
  }

  // Shadow right edge (incl bottom-right corner)
  for (int y = 1; y < frame_size.y() + 2; y++)
  {
    sprite_manager.render_icon(Icon::ICON_FRAME_SHADOW, frame_pos + geometry::Position((frame_size.x() + 1) * CHAR_W, y * CHAR_H));
  }
  // Shadow bottom edge
  for (int x = 1; x < frame_size.x() + 1; x++)
  {
    sprite_manager.render_icon(Icon::ICON_FRAME_SHADOW, frame_pos + geometry::Position(x * CHAR_W, (frame_size.y() + 1) * CHAR_H));
  }

  // Draw text
  int y = frame_pos.y() + 2 * CHAR_H;
  int x = frame_pos.x() + 2 * CHAR_W;
  for (const auto& string : strings_)
  {
    sprite_manager.render_text(string, geometry::Position(x, y));
    y += CHAR_H;
  }

  // Spinning question mark
  if (question_pos_ != geometry::Position(0, 0))
  {
    constexpr Icon q_icons[]{Icon::ICON_QUESTION_1,
                             Icon::ICON_QUESTION_2,
                             Icon::ICON_QUESTION_4,
                             Icon::ICON_QUESTION_3,
                             Icon::ICON_QUESTION_1,
                             Icon::ICON_QUESTION_3,
                             Icon::ICON_QUESTION_4,
                             Icon::ICON_QUESTION_2};
    constexpr bool flip[]{false, false, false, false, true, false, false, false};
    const auto icon_frame = (ticks_ / 2) % std::size(q_icons);
    sprite_manager.render_icon(q_icons[icon_frame], frame_pos + question_pos_, flip);
  }

  // Sparkle
  const auto sparkle_frame = (ticks_ / 3) % (std::size(S_ICONS) + 1);
  if (sparkle_frame > 0)
  {
    sprite_manager.render_icon(S_ICONS[sparkle_frame - 1], frame_pos + sparkle_pos_);
  }
}
