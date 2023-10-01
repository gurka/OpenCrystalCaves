/*
https://moddingwiki.shikadi.net/wiki/ProGraphx_Toolbox_tileset_format
*/
#include <filesystem>
#include <fstream>
#include <iostream>

#include <find_steam_game.h>

#define GOG_ID "1207665273"
#define GAME_NAME "Crystal Caves"

struct Header
{
  uint8_t count;
  uint8_t width;
  uint8_t height;
};

const std::string colors[] = {
  "⚫",
  "🔵",
  "🟢",
  "💧",
  "🔴",
  "🟣",
  "🟠",
  "⚪",
  "⬛",
  "🟦",
  "🟩",
  "🐬",
  "🟥",
  "🟪",
  "🟨",
  "⬜",
};

int main()
{
  char buf[4096];
  fsg_get_gog_game_path(buf,
#ifdef _WIN32
                        GOG_ID

#else
                        GAME_NAME
#endif
  );
  if (!buf[0])
  {
    std::cout << "Cannot find game data!\n";
    return 1;
  }
  const auto path = std::filesystem::path(buf)
#ifdef __APPLE__
    / "game"
#endif
    / "CC1.GFX";
  std::ifstream input{path, std::ios::binary};
  Header header;
  while (input.read(reinterpret_cast<char*>(&header), sizeof header))
  {
    std::cout << +header.count << "," << +header.width << "," << +header.height << "\n";
    const auto size = header.count * header.width * header.height * 5;
    if (size > 0)
    {
      std::string pixels(size, '\0');
      input.read(&pixels[0], size);
      uint8_t* pp = (uint8_t*)(&pixels[0]);
      for (int c = 0; c < header.count; c++)
      {
        for (int h = 0; h < header.height; h++)
        {
          for (int w = 0; w < header.width; w++)
          {
            const uint8_t t_plane = *pp++;
            const uint8_t b_plane = *pp++;
            const uint8_t g_plane = *pp++;
            const uint8_t r_plane = *pp++;
            const uint8_t i_plane = *pp++;
            for (int bit = 7; bit >= 0; bit--)
            {
              const bool t = (t_plane >> bit) & 1;
              if (!t)
              {
                std::cout << colors[0];
              }
              else
              {
                const bool b = (b_plane >> bit) & 1;
                const bool g = (g_plane >> bit) & 1;
                const bool r = (r_plane >> bit) & 1;
                const bool i = (i_plane >> bit) & 1;
                std::cout << colors[((int)i << 3) | ((int)r << 2) | ((int)g << 1) | (int)b];
              }
            }
          }
          std::cout << "\n";
        }
      }
    }
  }
  return 0;
}
