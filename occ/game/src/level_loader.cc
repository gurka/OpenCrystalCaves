#include "level_loader.h"

#include <cstdio>
#include <fstream>
#include <unordered_map>
#include <utility>

#include "game.h"
#include "level.h"
#include "logger.h"

namespace LevelLoader
{

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
const std::pair<Sprite, geometry::Size> levelBGs[] = {
  // intro
  {Sprite::SPRITE_STARS_1, {6, 1}},
  // finale
  {Sprite::SPRITE_STARS_1, {6, 1}},
  // main
  {Sprite::SPRITE_ROCKS_1, {2, 2}},
  {Sprite::SPRITE_HEX_ROCKS_1, {4, 2}},
  {Sprite::SPRITE_VERTICAL_WALL_1, {2, 2}},
  {Sprite::SPRITE_COBBLE_1, {2, 2}},
  {Sprite::SPRITE_RED_PANEL_1, {2, 2}},
  {Sprite::SPRITE_PEBBLE_WALL_1, {2, 2}},
  {Sprite::SPRITE_DARK_STONE_1, {3, 2}},
  {Sprite::SPRITE_DIAMOND_WALL_1, {2, 2}},
  {Sprite::SPRITE_COLUMN_AND_KNOB_1, {2, 2}},
  {Sprite::SPRITE_GREEN_SCAFFOLD_1, {3, 2}},
  {Sprite::SPRITE_WOOD_WALL_1, {4, 2}},
  {Sprite::SPRITE_GREY_STONE_1, {2, 2}},
  {Sprite::SPRITE_RED_RECT_1, {4, 2}},
  {Sprite::SPRITE_BRICK_1, {2, 2}},
  {Sprite::SPRITE_RED_SCAFFOLD_1, {4, 2}},
  {Sprite::SPRITE_METAL_BARS_1, {2, 2}},
  {Sprite::SPRITE_BLUE_DIAMOND_1, {2, 2}},
};
// Different levels use different block colours
const Sprite blockColors[] = {
  // Intro 1-2
  Sprite::SPRITE_BLOCK_PEBBLE_NW,
  Sprite::SPRITE_BLOCK_PEBBLE_NW,
  // Main
  Sprite::SPRITE_BLOCK_PEBBLE_NW,
  // 1-8
  Sprite::SPRITE_BLOCK_BROWN_NW,
  Sprite::SPRITE_BLOCK_CYAN_NW,
  Sprite::SPRITE_BLOCK_PEBBLE_NW,
  Sprite::SPRITE_BLOCK_CYAN_NW,
  Sprite::SPRITE_BLOCK_CYAN_NW,
  Sprite::SPRITE_BLOCK_PEBBLE_NW,
  Sprite::SPRITE_BLOCK_PEBBLE_NW,
  Sprite::SPRITE_BLOCK_BLUE_NW,
  // 9-16
  Sprite::SPRITE_BLOCK_PEBBLE_NW,
  Sprite::SPRITE_BLOCK_PEBBLE_NW,
  Sprite::SPRITE_BLOCK_BLUE_NW,
  Sprite::SPRITE_BLOCK_BLUE_NW,
  Sprite::SPRITE_BLOCK_GREEN_NW,
  Sprite::SPRITE_BLOCK_PEBBLE_NW,
  Sprite::SPRITE_BLOCK_METAL_NW,
};
std::vector<Sprite> STARS{
  // The sprite with the bright star (3) seems to be less common...
  Sprite::SPRITE_STARS_1, Sprite::SPRITE_STARS_1, Sprite::SPRITE_STARS_1, Sprite::SPRITE_STARS_1, Sprite::SPRITE_STARS_2,
  Sprite::SPRITE_STARS_2, Sprite::SPRITE_STARS_2, Sprite::SPRITE_STARS_2, Sprite::SPRITE_STARS_3, Sprite::SPRITE_STARS_4,
  Sprite::SPRITE_STARS_4, Sprite::SPRITE_STARS_4, Sprite::SPRITE_STARS_4, Sprite::SPRITE_STARS_5, Sprite::SPRITE_STARS_5,
  Sprite::SPRITE_STARS_5, Sprite::SPRITE_STARS_5, Sprite::SPRITE_STARS_6, Sprite::SPRITE_STARS_6, Sprite::SPRITE_STARS_6,
  Sprite::SPRITE_STARS_6,
};
std::vector<Sprite> HORIZON{
  Sprite::SPRITE_HORIZON_LAMP,
  Sprite::SPRITE_HORIZON_1,
  Sprite::SPRITE_HORIZON_2,
  Sprite::SPRITE_HORIZON_3,
  Sprite::SPRITE_HORIZON_4,
};

std::unique_ptr<Level> load(const ExeData& exe_data, const LevelId level_id)
{
  LOG_INFO("Loading level %d", static_cast<int>(level_id));
  // Find the location in exe data of the level
  const char* ptr = exe_data.data.c_str() + levelLoc;
  int level;
  for (level = static_cast<int>(LevelId::INTRO); level <= static_cast<int>(LevelId::LEVEL_16); level++)
  {
    if (level == static_cast<int>(level_id))
    {
      break;
    }
    // Skip this level's rows
    for (int row = 0; row < levelRows[level]; row++)
    {
      const size_t len = *ptr;
      ptr++;
      ptr += len;
    }
  }

  // Read the tile ids of the level
  std::vector<int> tile_ids;
  int width = 0;
  for (int row = 0; row < levelRows[level]; row++)
  {
    const int len = *ptr;
    if (width == 0)
    {
      width = len;
    }
    ptr++;
    const auto row_str = std::string(ptr).substr(0, len);
    LOG_DEBUG("%s", row_str.c_str());
    for (int i = 0; i < len; i++, ptr++)
    {
      tile_ids.push_back(static_cast<int>(*ptr));
    }
  }
  geometry::Position player_spawn;
  const auto background = levelBGs[static_cast<int>(level_id)];
  const auto block_sprite = blockColors[static_cast<int>(level_id)];
  std::vector<int> bgs;
  std::vector<Tile> tiles;
  std::vector<Item> items;
  std::vector<MovingPlatform> moving_platforms;
  std::vector<Entrance> entrances;

  bool has_earth = false;
  bool has_moon = false;
  bool is_stars_row = false;
  bool is_horizon_row = false;
  bool is_bumpable_platform = false;
  bool is_sign = false;
  bool is_wood_strut = false;
  bool is_wood_pillar = false;
  bool is_volcano = false;
  int volcano_sprite = -1;
  int entrance_level = static_cast<int>(LevelId::LEVEL_1);
  for (int i = 0; i < static_cast<int>(tile_ids.size()); i++)
  {
    const int x = i % width;
    if (x == 0)
    {
      is_stars_row = false;
      is_horizon_row = false;
      is_bumpable_platform = false;
      is_sign = false;
      is_wood_strut = false;
      is_wood_pillar = false;
      is_volcano = false;
      volcano_sprite = -1;
    }
    const int y = i / width;
    const auto tile_id = tile_ids[i];
    Tile tile;
    int bg = static_cast<int>(background.first);
    if (is_stars_row)
    {
      bg = static_cast<int>(STARS[rand() % STARS.size()]);
    }
    else if (is_horizon_row)
    {
      bg = static_cast<int>(HORIZON[rand() % HORIZON.size()]);
    }
    else if (background.first != Sprite::SPRITE_NONE)
    {
      // Normal background
      bg = static_cast<int>(background.first) + (((y + 1) % background.second.y()) * 4) + (x % background.second.x());
    }
    // Decode tile ids from exe data
    int sprite = -1;
    int sprite_count = 1;
    int flags = 0;
    auto item = Item::INVALID;
    if (is_bumpable_platform)
    {
      switch (tile_id)
      {
        case 'd':
        case -103:
          sprite = static_cast<int>(Sprite::SPRITE_BUMP_PLATFORM_RED_MID);
          flags |= TILE_SOLID;
          if (tile_ids[i] == -103)
          {
            // TODO: add hidden crystal
          }
          break;
        case 'n':
        case -102:
          sprite = static_cast<int>(Sprite::SPRITE_BUMP_PLATFORM_RED_MID);
          flags |= TILE_SOLID;
          if (tile_ids[i] == -102)
          {
            // TODO: add hidden crystal
          }
          is_bumpable_platform = false;
          break;
        default:
          break;
      }
    }
    else if (is_sign)
    {
      switch (tile_id)
      {
          // [4n = winners drugs sign
        case '4':
          sprite = static_cast<int>(Sprite::SPRITE_WINNERS_2);
          flags |= TILE_SOLID_TOP;
          break;
        case 'n':
          sprite = static_cast<int>(Sprite::SPRITE_WINNERS_3);
          flags |= TILE_SOLID_TOP;
          is_sign = false;
          break;
          // [m = mine sign
        case 'm':
          sprite = static_cast<int>(Sprite::SPRITE_MINE_SIGN_2);
          flags |= TILE_RENDER_IN_FRONT;
          is_sign = false;
          break;
          // [d = danger sign
        case 'd':
          sprite = static_cast<int>(Sprite::SPRITE_DANGER_2);
          flags |= TILE_SOLID_TOP;
          is_sign = false;
          break;
        default:
          break;
      }
    }
    else if (is_wood_strut)
    {
      switch (tile_id)
      {
        case 'n':
          sprite = static_cast<int>(Sprite::SPRITE_WOOD_STRUT_2);
          is_wood_strut = false;
          break;
        default:
          break;
      }
    }
    else if (is_wood_pillar)
    {
      switch (tile_id)
      {
        case 'n':
          sprite = static_cast<int>(Sprite::SPRITE_WOOD_PILLAR_2);
          is_wood_pillar = false;
          break;
        default:
          break;
      }
    }
    else if (is_volcano)
    {
      sprite = volcano_sprite;
      volcano_sprite++;
      if (tile_ids[i + 1] != 'n')
      {
        is_volcano = false;
        volcano_sprite = -1;
      }
    }
    else
    {
      switch (tile_id)
      {
        case ' ':
          break;
          // Crystals
        case '+':
          item = Item(Sprite::SPRITE_CRYSTAL_1_Y, ItemType::ITEM_TYPE_CRYSTAL, 0);
          break;
        case 'b':
          item = Item(Sprite::SPRITE_CRYSTAL_1_G, ItemType::ITEM_TYPE_CRYSTAL, 0);
          break;
        case 'R':
          item = Item(Sprite::SPRITE_CRYSTAL_1_R, ItemType::ITEM_TYPE_CRYSTAL, 0);
          break;
        case 'c':
          item = Item(Sprite::SPRITE_CRYSTAL_1_B, ItemType::ITEM_TYPE_CRYSTAL, 0);
          break;
          // Ammo
        case 'G':
          item = Item(Sprite::SPRITE_PISTOL, ItemType::ITEM_TYPE_AMMO, AMMO_AMOUNT);
          break;
          // Blocks
        case 'r':
          sprite = static_cast<int>(block_sprite);  // NW
          flags |= TILE_SOLID;
          break;
        case 't':
          sprite = static_cast<int>(block_sprite) + 1;  // N
          flags |= TILE_SOLID;
          break;
        case 'y':
          sprite = static_cast<int>(block_sprite) + 2;  // NE
          flags |= TILE_SOLID;
          break;
        case '4':
          sprite = static_cast<int>(block_sprite) + 8;  // W
          flags |= TILE_SOLID;
          break;
        case '5':
          sprite = static_cast<int>(block_sprite) + 9;  // MID
          flags |= TILE_SOLID;
          break;
        case '6':
          sprite = static_cast<int>(block_sprite) + 10;  // E
          flags |= TILE_SOLID;
          break;
        case 'f':
          sprite = static_cast<int>(block_sprite) + 4;  // SW
          flags |= TILE_SOLID;
          break;
        case 'g':
          sprite = static_cast<int>(block_sprite) + 5;  // S
          flags |= TILE_SOLID;
          break;
        case 'h':
          sprite = static_cast<int>(block_sprite) + 6;  // SE
          flags |= TILE_SOLID;
          break;
          // Bumpable platforms
        case 'D':
        case -104:
          // Keep adding bumpable platforms until we get an 'n'
          sprite = static_cast<int>(Sprite::SPRITE_BUMP_PLATFORM_RED_L);
          flags |= TILE_SOLID;
          if (tile_id == -104)
          {
            // TODO: add hidden crystal
          }
          is_bumpable_platform = true;
          break;
        case 'd':
          sprite = static_cast<int>(Sprite::SPRITE_BUMP_PLATFORM_RED_MID);
          flags |= TILE_SOLID;
          break;
        case 'H':
          moving_platforms.push_back({
            geometry::Position{x * 16, y * 16},
            true,
          });
          break;
        case 'k':
          sprite = static_cast<int>(Sprite::SPRITE_CONCRETE_V);
          flags |= TILE_SOLID;
          break;
        case 'K':
          sprite = static_cast<int>(Sprite::SPRITE_CONCRETE);
          flags |= TILE_SOLID;
          break;
        case 'l':
          sprite = static_cast<int>(Sprite::SPRITE_CONCRETE_X);
          flags |= TILE_SOLID;
          break;
        case 'L':
          sprite = static_cast<int>(Sprite::SPRITE_CONCRETE_H);
          flags |= TILE_SOLID;
          break;
        case 'm':
          has_earth = true;
          break;
        case 'n':
          // TODO: volcano spawn point?
          sprite = static_cast<int>(Sprite::SPRITE_VOLCANO_EJECTA_L_1);
          sprite_count = 4;
          flags |= TILE_ANIMATED;
          break;
        case 'N':
          has_moon = true;
          break;
        case 'u':
          // TODO: volcano spawn point?
          sprite = static_cast<int>(Sprite::SPRITE_VOLCANO_EJECTA_R_1);
          sprite_count = 4;
          flags |= TILE_ANIMATED;
          break;
        case 'V':
          moving_platforms.push_back({
            geometry::Position{x * 16, y * 16},
            false,
          });
          break;
        case 'x':
          // TODO: remember completion state
          // Everything is under construction...
          sprite = static_cast<int>(Sprite::SPRITE_CONES);
          flags |= TILE_RENDER_IN_FRONT;
          entrances.push_back({geometry::Position{x * 16, y * 16}, entrance_level, EntranceState::CLOSED});
          entrance_level++;
          break;
        case 'Y':
          // Player spawn
          player_spawn = geometry::Position(x * 16, y * 16);
          break;
        case 'z':
          if (is_horizon_row || (x == 0 && tile_ids[i + 1] == 'Z'))
          {
            // Random horizon tile
            bg = static_cast<int>(HORIZON[rand() % HORIZON.size()]);
            is_horizon_row = true;
          }
          else
          {
            // Random star tile
            bg = static_cast<int>(STARS[rand() % STARS.size()]);
            is_stars_row = true;
          }
          break;
        case 'Z':
          break;
        case '[':
          switch (tile_ids[i + 1])
          {
              // [4n = winners drugs sign
            case '4':
              sprite = static_cast<int>(Sprite::SPRITE_WINNERS_1);
              flags |= TILE_SOLID_TOP;
              is_sign = true;
              break;
              // [m = mine sign
            case 'm':
              sprite = static_cast<int>(Sprite::SPRITE_MINE_SIGN_1);
              flags |= TILE_RENDER_IN_FRONT;
              is_sign = true;
              break;
              // [d = danger sign
            case 'd':
              sprite = static_cast<int>(Sprite::SPRITE_DANGER_1);
              flags |= TILE_SOLID_TOP;
              is_sign = true;
              break;
            default:
              break;
          }
          break;
        case -6:
          sprite = static_cast<int>(Sprite::SPRITE_BARREL_CRACKED);
          flags |= TILE_SOLID_TOP;
          break;
        case -7:
          sprite = static_cast<int>(Sprite::SPRITE_BARREL);
          flags |= TILE_SOLID_TOP;
          break;
        case -16:
          if (tile_ids[i + 1] == 'n')
          {
            // Wood struts
            sprite = static_cast<int>(Sprite::SPRITE_WOOD_STRUT_1);
            is_wood_strut = true;
          }
          break;
        case -43:
          // TODO: animated
          sprite = static_cast<int>(Sprite::SPRITE_TORCH_1);
          sprite_count = 4;
          flags |= TILE_ANIMATED;
          break;
        case -58:
          sprite = static_cast<int>(Sprite::SPRITE_SIGN_DOWN);
          break;
        case -77:
          if (tile_ids[i + 1] == 'n')
          {
            // Wood pillar
            sprite = static_cast<int>(Sprite::SPRITE_WOOD_PILLAR_1);
            is_wood_pillar = true;
          }
          break;
        case -78:
          sprite = static_cast<int>(Sprite::SPRITE_WOOD_V);
          flags |= TILE_SOLID;
          break;
        case -79:
          sprite = static_cast<int>(Sprite::SPRITE_WOOD_H);
          flags |= TILE_SOLID;
          break;
        case -113:
          if (tile_ids[i + 1] == 'n')
          {
            // -113 nnn = bottom of volcano
            sprite = static_cast<int>(Sprite::SPRITE_VOLCANO_BOTTOM_1);
            is_volcano = true;
            volcano_sprite = sprite + 1;
          }
          break;
        case -114:
          if (tile_ids[i + 1] == 'n')
          {
            // -114 n = top of volcano
            sprite = static_cast<int>(Sprite::SPRITE_VOLCANO_TOP_1);
            is_volcano = true;
            volcano_sprite = sprite + 1;
          }
          break;
        default:
          break;
      }
    }
    if (sprite == -1)
    {
      tile = Tile::INVALID;
    }
    else
    {
      tile = Tile(sprite, sprite_count, flags);
    }
    tiles.push_back(tile);
    bgs.push_back(bg);
    items.push_back(item);
  }

  return std::make_unique<Level>(level_id,
                                 width,
                                 levelRows[static_cast<int>(level_id)],
                                 player_spawn,
                                 std::move(moving_platforms),
                                 std::move(entrances),
                                 std::move(bgs),
                                 std::move(tiles),
                                 std::move(items),
                                 has_earth,
                                 has_moon);
}

}
