/*
Display Crystal Caves levels
*/
#include <filesystem>
#include <fstream>
#include <iostream>

#include "../game/src/level.h"
#include "../game/src/level_loader.h"
#include "../game/src/object_manager.h"
#include "../occ/src/constants.h"
#include "../occ/src/spritemgr.h"
#include "../utils/export/exe_data.h"
#include "event.h"
#include "graphics.h"
#include "logger.h"
#include "sdl_wrapper.h"

static constexpr geometry::Size WIN_SIZE = geometry::Size(40 * SPRITE_W, 25 * SPRITE_H);

// https://moddingwiki.shikadi.net/wiki/Crystal_Caves_Map_Format

constexpr int levelLoc = 0x8CE0;
constexpr int levelRows[] = {
  // intro
  5,
  // finale
  6,
  // main
  25,
  24,
  24,
  24,
  24,
  24,
  24,
  23,
  23,
  24,
  24,
  24,
  24,
  24,
  23,
  24,
  24,
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
  auto window = Window::create("Level Viewer", WIN_SIZE, "");
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
  ObjectManager object_manager;
  if (!object_manager.load("media/items.json"))
  {
    LOG_CRITICAL("Could not load items");
    return 1;
  }
  ExeData exe_data{episode};
  std::vector<Level> levels;
  // TODO: read exe data
  auto level = LevelLoader::load_level(LevelId::LEVEL_ONE);
  levels.push_back(*level);
  int index = 0;
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
    if (input.left.pressed() || input.up.pressed())
    {
      index--;
      if (index < 0)
      {
        index = (int)levels.size() - 1;
      }
    }
    else if (input.right.pressed() || input.down.pressed())
    {
      index++;
      if (index == (int)levels.size())
      {
        index = 0;
      }
    }

    window->fill_rect(geometry::Rectangle(0, 0, WIN_SIZE), {33u, 33u, 33u});
    const auto& bg = object_manager.get_background(levels[index].background_id);
    for (int y = 0; y < levels[index].height; y++)
    {
      for (int x = 0; x < levels[index].width; x++)
      {
        if (bg.valid())
        {
          const auto sprite_id = bg.get_sprite(x, y);
          sprite_manager.render_tile(sprite_id, {x * SPRITE_W, y * SPRITE_H});
        }
        const auto tile_id = levels[index].tile_ids[y * levels[index].width + x];
        if (tile_id != -1)
        {
          const auto& tile = object_manager.get_tile(tile_id);
          if (tile.valid())
          {
            const auto sprite_id = tile.get_sprite();
            sprite_manager.render_tile(sprite_id, {x * SPRITE_W, y * SPRITE_H});
          }
        }
      }
    }
    window->refresh();
    sdl->delay(10);
  }

  return 0;
}
