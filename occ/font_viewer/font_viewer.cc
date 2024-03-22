/*
Show example text using the Crystal Caves font(s)
*/
#include <filesystem>
#include <fstream>
#include <iostream>

#include "../occ/src/spritemgr.h"
#include "event.h"
#include "graphics.h"
#include "logger.h"
#include "sdl_wrapper.h"

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
  auto window = Window::create("OpenCrystalCaves", geometry::Size(10, 10), "");
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
  const auto text = L"This is the Crystal Caves font!\n"
                    "\n"
                    "The quick brown fox jumps over the lazy dog\n"
                    "Sphinx of black quartz, judge my vow.\n\n"
                    "ABCDEFGHIJKLMNOPQRSTUVWXYZ\n"
                    "abcdefghijklmnopqrstuvwxyz\n"
                    "0123456789";
  const auto size = geometry::Size(400, 100);
  window->set_size(size);
  auto event = Event::create();
  if (!event)
  {
    LOG_CRITICAL("Could not create event handler");
    return 1;
  }

  Input input;
  while (true)
  {
    event->poll_event(&input);
    if (input.escape.pressed())
    {
      break;
    }
    window->fill_rect(geometry::Rectangle(0, 0, size), {33u, 33u, 33u});
    sprite_manager.render_text(text, geometry::Position(5, 5));
    window->refresh();
    sdl->delay(10);
  }

  return 0;
}
