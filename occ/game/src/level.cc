#include "level.h"

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

Level::Level(LevelId level_id,
             int width,
             int height,
             const Sprite bg_sprite,
             geometry::Size bg_tile_size,
             const std::vector<int>& tile_ids,
             std::vector<int>&& item_ids)
  : level_id(level_id),
    width(width),
    height(height),
    item_ids(std::move(item_ids)),
    moving_platforms(),
    entrances()
{
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
    int bg = static_cast<int>(bg_sprite);
    if (is_stars_row)
    {
      bg = static_cast<int>(STARS[rand() % STARS.size()]);
    }
    else if (is_horizon_row)
    {
      bg = static_cast<int>(HORIZON[rand() % HORIZON.size()]);
    }
    else if (bg_sprite != Sprite::SPRITE_NONE)
    {
      // Normal background
      bg = static_cast<int>(bg_sprite) + (((y + 1) % bg_tile_size.y()) * 4) + (x % bg_tile_size.x());
    }
    // Decode tile ids from exe data
    int sprite = -1;
    int sprite_count = 1;
    int flags = 0;
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
          // Blocks
        case 'r':
          sprite = static_cast<int>(Sprite::SPRITE_BLOCK_BROWN_NW);
          flags |= TILE_SOLID;
          break;
        case 't':
          sprite = static_cast<int>(Sprite::SPRITE_BLOCK_BROWN_N);
          flags |= TILE_SOLID;
          break;
        case 'y':
          sprite = static_cast<int>(Sprite::SPRITE_BLOCK_BROWN_NE);
          flags |= TILE_SOLID;
          break;
        case '4':
          sprite = static_cast<int>(Sprite::SPRITE_BLOCK_BROWN_W);
          flags |= TILE_SOLID;
          break;
        case '5':
          sprite = static_cast<int>(Sprite::SPRITE_BLOCK_BROWN_MID);
          flags |= TILE_SOLID;
          break;
        case '6':
          sprite = static_cast<int>(Sprite::SPRITE_BLOCK_BROWN_E);
          flags |= TILE_SOLID;
          break;
        case 'f':
          sprite = static_cast<int>(Sprite::SPRITE_BLOCK_BROWN_SW);
          flags |= TILE_SOLID;
          break;
        case 'g':
          sprite = static_cast<int>(Sprite::SPRITE_BLOCK_BROWN_S);
          flags |= TILE_SOLID;
          break;
        case 'h':
          sprite = static_cast<int>(Sprite::SPRITE_BLOCK_BROWN_SE);
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
          this->moving_platforms.push_back({
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
          this->moving_platforms.push_back({
            geometry::Position{x * 16, y * 16},
            false,
          });
          break;
        case 'x':
          // TODO: remember completion state
          this->entrances.push_back({geometry::Position{x * 16, y * 16}, entrance_level, EntranceState::CLOSED});
          entrance_level++;
          break;
        case 'Y':
          // Player spawn
          this->player_spawn = geometry::Position(x * 16, y * 16);
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
  }
}

const Tile& Level::get_tile(const int x, const int y) const
{
  if (x < 0 || y >= width || y < 0 || y >= height)
  {
    return Tile::INVALID;
  }
  return tiles[(y * width) + x];
}

const int Level::get_bg(const int x, const int y) const
{
  if (x < 0 || y >= width || y < 0 || y >= height)
  {
    return -1;
  }
  return bgs[(y * width) + x];
}
