/*
https://moddingwiki.shikadi.net/wiki/ProGraphx_Toolbox_tileset_format
*/
#include "spritemgr.h"

#include <filesystem>
#include <fstream>

#include "logger.h"
#include "occ_math.h"
#include "path.h"

#define GFX_FILENAME_FMT "CC{}.GFX"
// TODO: multiple font files
#define CHAR_FILENAME_FMT "CC{}-F1.MNI"
#define FILLER 2
#define SPRITE_W 16
#define SPRITE_H 16
#define SPRITE_STRIDE 52
#define CHAR_W 8
#define CHAR_H 8
#define CHAR_STRIDE 50

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

int read_sprite_count(std::ifstream& input, const int filler)
{
	Header header;
	int count = 0;
	while (input.read(reinterpret_cast<char*>(&header), sizeof header))
	{
		count += header.count;
		if (header.count > 0)
		{
			count += filler;
		}
		const auto size = header.count * header.width * header.height * 5;
		input.seekg(size, std::ios_base::cur);
	}
	input.clear();
	input.seekg(0, std::ios_base::beg);
	return count;
}

std::unique_ptr<Surface> load_surface(const std::filesystem::path& path, Window& window, const int sprite_w, const int sprite_h, const int stride, const int filler)
{
	if (path.empty())
	{
		LOG_CRITICAL("Could not find game data!");
		return nullptr;
	}
	std::ifstream input{path, std::ios::binary};
	const int count = read_sprite_count(input, filler);
	if (count == 0)
	{
		LOG_CRITICAL("Could not load any sprites!");
		return nullptr;
	}
	const int sheet_w =stride*sprite_w;
	const int sheet_h = math::round_up(count*sprite_h/stride, sprite_h);
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
		  int x_start = (index % stride) * sprite_w;
		  int y_start = (index / stride) * sprite_h;
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
				const int pixel_i = x + y * stride * sprite_h;
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
				if (x == x_start + sprite_w)
				{
					y++;
					x = x_start;
				}
			}
		  }
		}
	  }
		  index += filler;
	}
  }
	auto surface = Surface::from_pixels(sheet_w, sheet_h, (uint32_t*)all_pixels.data(), window);
	if (!surface)
	{
	  LOG_CRITICAL("Could not load '%s'", path.c_str());
	}
	return surface;
}

std::unique_ptr<Surface> load_tiles(Window& window, const int episode)
{
	// Load tileset
  // TODO: use <format> in C++20
  const auto path = get_data_path(std::string(GFX_FILENAME_FMT).replace(std::string(GFX_FILENAME_FMT).find("{}"), std::string("{}").length(), std::to_string(episode)));
  return load_surface(path, window,SPRITE_W, SPRITE_H, SPRITE_STRIDE, FILLER);
}
								  
std::unique_ptr<Surface> load_chars(Window& window, const int episode)
{
  // Load fonts/characters
// TODO: use <format> in C++20
const auto path = get_data_path(std::string(CHAR_FILENAME_FMT).replace(std::string(CHAR_FILENAME_FMT).find("{}"), std::string("{}").length(), std::to_string(episode)));
return load_surface(path, window,CHAR_W, CHAR_H, CHAR_STRIDE, 0);
}

bool SpriteManager::load_tilesets(Window& window, const int episode)
{
  if (!sprite_surface_)
  {
	  sprite_surface_ = load_tiles(window, episode);
	  if (!sprite_surface_)
	  {
		return false;
	  }
  }
  if (!char_surface_)
   {
	   char_surface_ = load_chars(window, episode);
	   if (!char_surface_)
	   {
		 return false;
	   }
   }

  return true;
}

const Surface* SpriteManager::get_surface() const
{
  return sprite_surface_.get();
}

geometry::Rectangle SpriteManager::get_rect_for_tile(const int sprite) const
{
  return
  {
    (sprite % (sprite_surface_->width() / SPRITE_W)) * SPRITE_W,
    (sprite / (sprite_surface_->width() / SPRITE_H)) * SPRITE_H,
	  SPRITE_W,
	  SPRITE_H
  };
}
									
const Surface* SpriteManager::get_char_surface() const
 {
   return char_surface_.get();
 }

 geometry::Rectangle SpriteManager::get_rect_for_char(const int ch) const
 {
   return
   {
	 (ch % (char_surface_->width() / CHAR_W)) * CHAR_W,
	 (ch / (char_surface_->width() / CHAR_H)) * CHAR_H,
	   CHAR_W,
	   CHAR_H
   };
 }
