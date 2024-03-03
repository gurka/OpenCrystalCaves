/*
Display Crystal Caves window panels
*/
#include <filesystem>
#include <fstream>
#include <iostream>

#include "../occ/src/constants.h"
#include "../occ/src/panel.h"
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
