/*
https://moddingwiki.shikadi.net/wiki/ProGraphx_Toolbox_tileset_format

Show the character/SPL spritesheets all in one
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
  const auto surface = sprite_manager.get_char_surface();
  window->set_size(surface->size());
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
    window->fill_rect(geometry::Rectangle(0, 0, surface->size()), {33u, 33u, 33u});
    surface->blit_surface();
    window->refresh();
    sdl->delay(10);
  }

  return 0;
}
