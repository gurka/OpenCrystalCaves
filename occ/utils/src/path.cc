#include "path.h"

#include <find_steam_game.h>

#define GOG_ID "1207665273"
#define GAME_NAME "Crystal Caves"

std::filesystem::path get_data_path(const std::filesystem::path& filename)
{
  // Try CWD first
  if (std::filesystem::exists(filename))
  {
    return std::filesystem::path(filename);
  }
  char buf[4096];
  // Try steam
  fsg_get_steam_game_path(buf, GAME_NAME);
  if (buf[0])
  {
    const auto steamfilename = std::filesystem::path(buf) / GAME_NAME / filename;
    if (std::filesystem::exists(steamfilename))
    {
      return std::filesystem::path(steamfilename);
    }
  }
  // Try GoG
  fsg_get_gog_game_path(buf,
#ifdef _WIN32
                        GOG_ID
#else
                        GAME_NAME
#endif
  );
  if (buf[0])
  {
    const auto gogfilename = std::filesystem::path(buf)
#ifdef __APPLE__
      / "game"
#endif
      / filename;
    if (std::filesystem::exists(gogfilename))
    {
      return std::filesystem::path(gogfilename);
    }
  }
  return std::filesystem::path();
}
