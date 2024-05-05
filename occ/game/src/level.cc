#include "level.h"

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
             const geometry::Position& player_spawn,
             const std::string& background,
             const std::vector<int>& tile_ids,
             std::vector<int>&& item_ids)
  : level_id(level_id),
    width(width),
    height(height),
    player_spawn(player_spawn),
    background(background),
    item_ids(std::move(item_ids)),
    moving_platforms()
{
  for (int i = 0; i < tile_ids.size(); i++)
  {
    const int x = i % width;
    const int y = i / width;
    const auto tile_id = tile_ids[i];
    Tile tile;
    // Decode tile ids from exe data
    int sprite = -1;
    int sprite_count = 1;
    int flags = 0;
    switch (tile_id)
    {
      case ' ':
        // Nothing
        sprite = -1;
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
        tiles.push_back(Tile(static_cast<int>(Sprite::SPRITE_BUMP_PLATFORM_RED_L), 1, TILE_SOLID));
        i++;
        if (tile_id == -104)
        {
          // TODO: add hidden crystal
        }
        for (; i < tile_ids.size() && tile_ids[i] != 'n' && tile_ids[i] != -102; i++)
        {
          tiles.push_back(Tile(static_cast<int>(Sprite::SPRITE_BUMP_PLATFORM_RED_MID), 1, TILE_SOLID));
          if (tile_ids[i] == -103)
          {
            // TODO: add hidden crystal
          }
        }
        tiles.push_back(Tile(static_cast<int>(Sprite::SPRITE_BUMP_PLATFORM_RED_R), 1, TILE_SOLID));
        if (tile_ids[i] == -102)
        {
          // TODO: add hidden crystal
        }
        continue;
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
        // mN = earth and moon
        if (tile_ids[i + 1] == 'N')
        {
          tiles.push_back(Tile(static_cast<int>(Sprite::SPRITE_EARTH), 1, 0));
          i++;
          tiles.push_back(Tile(static_cast<int>(Sprite::SPRITE_MOON), 1, 0));
          continue;
        }
        break;
      case 'n':
        // TODO: volcano spawn point?
        sprite = static_cast<int>(Sprite::SPRITE_VOLCANO_EJECTA_L_1);
        sprite_count = 4;
        flags |= TILE_ANIMATED;
        break;
      case 'N':
        // TODO: earth?
        sprite = static_cast<int>(Sprite::SPRITE_EARTH);
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
        // TODO: entrance
        sprite = static_cast<int>(Sprite::SPRITE_ENTRY_1);
        break;
      case 'Y':
        // Player spawn
        this->player_spawn = geometry::Position(x * 16, y * 16);
        break;
      case 'z':
        // TODO: stars
        sprite = static_cast<int>(Sprite::SPRITE_STARS);
        break;
      case 'Z':
        // Random horizon tile
        sprite = static_cast<int>(HORIZON[rand() % HORIZON.size()]);
        break;
      case '[':
        switch (tile_ids[i + 1])
        {
            // [4n = winners drugs sign
          case '4':
            tiles.push_back(Tile(static_cast<int>(Sprite::SPRITE_WINNERS_1), 1, TILE_SOLID_TOP));
            i++;
            tiles.push_back(Tile(static_cast<int>(Sprite::SPRITE_WINNERS_2), 1, TILE_SOLID_TOP));
            i++;
            tiles.push_back(Tile(static_cast<int>(Sprite::SPRITE_WINNERS_3), 1, TILE_SOLID_TOP));
            continue;
            // [m = mine sign
          case 'm':
            tiles.push_back(Tile(static_cast<int>(Sprite::SPRITE_MINE_SIGN_1), 1, 0));
            i++;
            tiles.push_back(Tile(static_cast<int>(Sprite::SPRITE_MINE_SIGN_2), 1, 0));
            continue;
            // [d = danger sign
          case 'd':
            tiles.push_back(Tile(static_cast<int>(Sprite::SPRITE_DANGER_1), 1, TILE_SOLID_TOP));
            i++;
            tiles.push_back(Tile(static_cast<int>(Sprite::SPRITE_DANGER_2), 1, TILE_SOLID_TOP));
            continue;
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
          tiles.push_back(Tile(static_cast<int>(Sprite::SPRITE_WOOD_STRUT_1), 1, 0));
          i++;
          tiles.push_back(Tile(static_cast<int>(Sprite::SPRITE_WOOD_STRUT_2), 1, 0));
          continue;
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
          tiles.push_back(Tile(static_cast<int>(Sprite::SPRITE_WOOD_PILLAR_1), 1, 0));
          i++;
          tiles.push_back(Tile(static_cast<int>(Sprite::SPRITE_WOOD_PILLAR_2), 1, 0));
          continue;
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
          // \tnnn = bottom of volcano
          tiles.push_back(Tile(static_cast<int>(Sprite::SPRITE_VOLCANO_BOTTOM_1), 1, 0));
          i++;
          tiles.push_back(Tile(static_cast<int>(Sprite::SPRITE_VOLCANO_BOTTOM_2), 1, 0));
          i++;
          tiles.push_back(Tile(static_cast<int>(Sprite::SPRITE_VOLCANO_BOTTOM_3), 1, 0));
          i++;
          tiles.push_back(Tile(static_cast<int>(Sprite::SPRITE_VOLCANO_BOTTOM_4), 1, 0));
          continue;
        }
        break;
      case -114:
        if (tile_ids[i + 1] == 'n')
        {
          // \tn = top of volcano
          tiles.push_back(Tile(static_cast<int>(Sprite::SPRITE_VOLCANO_TOP_1), 1, 0));
          i++;
          tiles.push_back(Tile(static_cast<int>(Sprite::SPRITE_VOLCANO_TOP_2), 1, 0));
          continue;
        }
        break;
      default:
        sprite = -1;
        break;
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
