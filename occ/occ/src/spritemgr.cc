/*
https://moddingwiki.shikadi.net/wiki/ProGraphx_Toolbox_tileset_format
*/
#include "spritemgr.h"

#include <filesystem>
#include <fstream>

#include <find_steam_game.h>

#include "logger.h"

#define GFX_FILENAME "CC1.GFX"
#define GOG_ID "1207665273"
#define GAME_NAME "Crystal Caves"
#define STRIDE 52
#define FILLER 2
#define SPRITE_W 16
#define SPRITE_H 16

struct Header
{
  uint8_t count;
  uint8_t width;
  uint8_t height;
};

const uint32_t colors[] = {
  0xFF000000, // "⚫",
  0xFF0000AA, // "🔵",
  0xFF00AA00, // "🟢",
  0xFF00AAAA, // "💧",
  0xFFAA0000, // "🔴",
  0xFFAA00AA, // "🟣",
  0xFFAA5500, // "🟠",
  0xFFAAAAAA, // "⚪",
  0xFF555555, // "⬛",
  0xFF5555FF, // "🟦",
  0xFF55FF55, // "🟩",
  0xFF55FFFF, // "🐬",
  0xFFFF5555, // "🟥",
  0xFFFF55FF, // "🟪",
  0xFFFFFFAA, // "🟨",
  0xFFFFFFFF, // "⬜",
};

std::filesystem::path get_graphics_path()
{
	// Try CWD first
	if (std::filesystem::exists(GFX_FILENAME))
	{
		return std::filesystem::path(GFX_FILENAME);
	}
	// Try GoG
	char buf[4096];
	fsg_get_gog_game_path(buf,
  #ifdef _WIN32
						  GOG_ID
  #else
						  GAME_NAME
  #endif
	);
	if (buf[0])
	{
		return std::filesystem::path(buf)
#ifdef __APPLE__
	/ "game"
#endif
	/ GFX_FILENAME;
	}
	// TODO: Try steam
	return std::filesystem::path();
}

int read_sprite_count(std::ifstream& input)
{
	Header header;
	int count = 0;
	while (input.read(reinterpret_cast<char*>(&header), sizeof header))
	{
		count += header.count;
		if (header.count > 0)
		{
			count += FILLER;
		}
		const auto size = header.count * header.width * header.height * 5;
		input.seekg(size, std::ios_base::cur);
	}
	input.clear();
	input.seekg(0, std::ios_base::beg);
	return count;
}

std::unique_ptr<Surface> load_surface(std::ifstream& input, const int count, Window& window)
{
	const int sheet_w =STRIDE*SPRITE_W;
	const int sheet_h =count*SPRITE_H/STRIDE;
	std::string all_pixels(sheet_w*sheet_h*sizeof(uint32_t), '\0');
  Header header;
	int index = 0;
  while (input.read(reinterpret_cast<char*>(&header), sizeof header))
  {
	const auto size = header.count * header.width * header.height * 5;
	if (size > 0)
	{
	  std::string pixels(size, '\0');
	  input.read(&pixels[0], size);
	  uint8_t* pp = (uint8_t*)(&pixels[0]);
	  for (int c = 0; c < header.count; c++, index++)
	  {
		  int x_start = (index % STRIDE) * SPRITE_W;
		  int y_start = (index / STRIDE) * SPRITE_H;
		  int x = x_start;
		  int y = y_start;
		for (int h = 0; h < header.height; h++)
		{
		  for (int w = 0; w < header.width; w++)
		  {
			const uint8_t t_plane = *pp++;
			const uint8_t b_plane = *pp++;
			const uint8_t g_plane = *pp++;
			const uint8_t r_plane = *pp++;
			const uint8_t i_plane = *pp++;
			  int pi = 0;
			for (int bit = 7; bit >= 0; bit--, pi++)
			{
			  const bool t = (t_plane >> bit) & 1;
				const int pixel_i = x + y * STRIDE * SPRITE_H;
			  if (!t)
			  {
				  ((uint32_t*)all_pixels.data())[pixel_i] = 0;
			  }
			  else
			  {
				const bool b = (b_plane >> bit) & 1;
				const bool g = (g_plane >> bit) & 1;
				const bool r = (r_plane >> bit) & 1;
				const bool i = (i_plane >> bit) & 1;
				  ((uint32_t*)all_pixels.data())[pixel_i] = colors[((int)i << 3) | ((int)r << 2) | ((int)g << 1) | (int)b];
			  }
				x++;
				if (x == x_start + SPRITE_W)
				{
					y++;
					x = x_start;
				}
			}
		  }
		}
	  }
		  index += FILLER;
	}
  }
	return Surface::from_pixels(sheet_w, sheet_h, (uint32_t*)all_pixels.data(), window);
}

bool SpriteManager::load_tileset(Window& window)
{
  // Check if tileset already loaded
  if (sprite_surface_)
  {
    return false;
  }

  // Load tileset
	const auto path = get_graphics_path();
	if (path.empty())
	{
		LOG_CRITICAL("Could not find game data!");
		return false;
	}
	std::ifstream input{path, std::ios::binary};
	const int count = read_sprite_count(input);
	if (count == 0)
	{
		LOG_CRITICAL("Could not load any sprites!");
		return false;
	}
  sprite_surface_ = load_surface(input, count, window);
  if (!sprite_surface_)
  {
    LOG_CRITICAL("Could not load '%s'", path.c_str());
    return false;
  }

  return true;
}

int SpriteManager::number_of_tiles() const
{
  return (sprite_surface_->width() / SPRITE_W) * (sprite_surface_->height() / SPRITE_H);
}

const Surface* SpriteManager::get_surface() const
{
  return sprite_surface_.get();
}

geometry::Rectangle SpriteManager::get_rect_for_tile(int sprite) const
{
  return
  {
    (sprite % (sprite_surface_->width() / SPRITE_W)) * SPRITE_W,
    (sprite / (sprite_surface_->width() / SPRITE_H)) * SPRITE_H,
	  SPRITE_W,
	  SPRITE_H
  };
}
