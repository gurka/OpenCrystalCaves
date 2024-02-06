/*
Show UI elements using the Crystal Caves font(s)
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
	
	const int score = 7000;
	const int ammo = 10;
	const int hearts = 3;

  Input input;
  while (!input.quit)
  {
    event->poll_event(&input);
    window->fill_rect(geometry::Rectangle(0, 0, size), {33u, 33u, 33u});
	// $
	sprite_manager.render_text(L"$", Vector<int>(0, 0));
	// score
	sprite_manager.render_number(score, Vector<int>(8 * CHAR_W, 0));
	// Gun
	  sprite_manager.render_icon(Icon::ICON_GUN, Vector<int>(11 * CHAR_W, 0));
	// ammo
	  sprite_manager.render_number(ammo, Vector<int>(15 * CHAR_W, 0));
	// Hearts
	  for (int i = 0; i < hearts; i++)
	  {
		  sprite_manager.render_icon(Icon::ICON_HEART, Vector<int>((i + 19) * CHAR_W, 0));
	  }
	  // Key
	  sprite_manager.render_icon(Icon::ICON_KEY, Vector<int>(23 * CHAR_W, 0));
    window->refresh();
	  sdl->delay(10);
  }

  return 0;
}
