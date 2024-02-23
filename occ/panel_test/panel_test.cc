/*
Display Crystal Caves window panels
*/
#include <codecvt>
#include <filesystem>
#include <fstream>
#include <iostream>

#include "../occ/src/constants.h"
#include "../occ/src/spritemgr.h"
#include "event.h"
#include "graphics.h"
#include "logger.h"
#include "misc.h"
#include "sdl_wrapper.h"

constexpr Icon S_ICONS[]{Icon::ICON_SPARKLE_1, Icon::ICON_SPARKLE_2, Icon::ICON_SPARKLE_3, Icon::ICON_SPARKLE_4};

class Panel
{
 public:
  Panel(const std::string& ucsd)
  {
    // Convert input text into a vector of strings, ignoring the
    // END OF WINDOW text
    // Also find the max string size to determine window size
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    constexpr size_t len_limit = 32;
    size_t max_len = 0;
    const char* p_ucsd = ucsd.c_str();
    while (true)
    {
      const size_t len = *p_ucsd;
      max_len = std::max(len, max_len);
      p_ucsd++;
      std::string s(p_ucsd, len);
      if (s == "END OF WINDOW")
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
        question_pos_ = geometry::Position((question + 2) * CHAR_W, (strings_.size() + 2) * CHAR_H);
      }
      strings_.push_back(converter.from_bytes(s));
      p_ucsd += len;
    }
    max_len = std::min(max_len, len_limit);
    size_ = geometry::Position(static_cast<int>(max_len) + 1, static_cast<int>(strings_.size()) + 1);
  }

  void update()
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

  void draw(const SpriteManager& sprite_manager) const
  {
    const geometry::Position frame_pos((SCREEN_SIZE.x() / CHAR_W - size_.x() - 1) / 2 * CHAR_W,
                                       (SCREEN_SIZE.y() / CHAR_H - size_.y() - 1) / 2 * CHAR_H);
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
      constexpr Icon q_icons[]{
        Icon::ICON_QUESTION_1, Icon::ICON_QUESTION_2, Icon::ICON_QUESTION_3, Icon::ICON_QUESTION_4, Icon::ICON_QUESTION_5};
      const auto icon_frame = (ticks_ / 2) % std::size(q_icons);
      sprite_manager.render_icon(q_icons[icon_frame], frame_pos + question_pos_);
    }

    // Sparkle
    const auto sparkle_frame = (ticks_ / 3) % (std::size(S_ICONS) + 1);
    if (sparkle_frame > 0)
    {
      sprite_manager.render_icon(S_ICONS[sparkle_frame - 1], frame_pos + sparkle_pos_);
    }
  }

 private:
  std::vector<std::wstring> strings_;
  geometry::Size size_;
  geometry::Position question_pos_ = {0, 0};
  geometry::Position sparkle_pos_ = {0, 0};
  unsigned ticks_ = 0;
};

int main(int argc, char* argv[])
{
  int episode = 1;
  if (argc > 1)
  {
    episode = atoi(argv[1]);
  }
  auto sdl = SDLWrapper::create();
  if (!sdl)
  {
    LOG_CRITICAL("Could not create SDLWrapper");
    return 1;
  }
  if (!sdl->init())
  {
    LOG_CRITICAL("Could not initialize SDLWrapper");
    return 1;
  }
  auto window = Window::create("OpenCrystalCaves", SCREEN_SIZE, "");
  if (!window)
  {
    LOG_CRITICAL("Could not create Window");
    return 1;
  }
  SpriteManager sprite_manager;
  if (!sprite_manager.load_tilesets(*window, episode))
  {
    LOG_CRITICAL("Could not load tilesets");
    return 1;
  }
  std::vector<Panel> panels = {{"\x19"
                                "Welcome to Crystal Caves!"
                                "\x16"
                                "   Quit to DOS (Y-N) ^"
                                "\x0D"
                                "END OF WINDOW"},
                               {"\x1A"
                                "     GALACTIC ENCYCLOPEDIA"
                                "\x1A"
                                "     ---------------------"
                                "\x1B"
                                "ENTRY # 981,231,783,813,651"
                                "\x15"
                                "NAME:  Mylo Steamwitz"
                                "\x0C"
                                "RACE:  Human"
                                "\x12"
                                "SSN:   496-WZ-3452"
                                "\x19"
                                "OCCUPATION:  Space Trader"
                                "\x1B"
                                "Mylo scours the universe in"
                                "\x1E"
                                "search of anything he can sell"
                                "\x1D"
                                "that will make him a buck and"
                                "\x19"
                                "keep him from joining his"
                                "\x1E"
                                "uncle@s Yorp herding business."
                                "\x17"
                                "        Press Any Key ^"
                                "\x0D"
                                "END OF WINDOW"}};
  int index = 0;
  auto event = Event::create();
  if (!event)
  {
    LOG_CRITICAL("Could not create event handler");
    return 1;
  }

  auto sdl_tick = sdl->get_tick();
  const auto ms_per_update = 57;  // 17.5~ ticks per second
  auto tick_last_update = sdl_tick;
  auto lag = 0u;
  Input input;
  while (!input.quit)
  {
    sdl_tick = sdl->get_tick();
    auto elapsed_ticks = sdl_tick - tick_last_update;
    tick_last_update = sdl_tick;
    lag += elapsed_ticks;
    while (lag >= ms_per_update)
    {
      event->poll_event(&input);
      if (input.left.pressed() || input.up.pressed())
      {
        index--;
        if (index < 0)
        {
          index = (int)panels.size() - 1;
        }
      }
      else if (input.right.pressed() || input.down.pressed())
      {
        index++;
        if (index == (int)panels.size())
        {
          index = 0;
        }
      }
      panels[index].update();

      lag -= ms_per_update;
    }

    window->fill_rect(geometry::Rectangle(0, 0, SCREEN_SIZE), {33u, 33u, 33u});
    panels[index].draw(sprite_manager);
    window->refresh();
    sdl->delay(10);
  }

  return 0;
}
