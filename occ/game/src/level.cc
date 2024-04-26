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
             std::vector<int>&& item_ids,
             std::vector<MovingPlatform>&& moving_platforms,
             const ObjectManager& object_manager,
             const bool decode)
  : level_id(level_id),
    width(width),
    height(height),
    player_spawn(player_spawn),
    background(background),
    item_ids(std::move(item_ids)),
    moving_platforms(std::move(moving_platforms))
{
  for (int i = 0; i < tile_ids.size(); i++)
  {
    const int x = i % width;
    const int y = i / width;
    const auto tile_id = tile_ids[i];
    Tile tile;
    if (decode)
    {
      // Decode tile ids from exe data
      int sprite = 0;
      switch (tile_id)
      {
        case 'H':
          // TODO: start and end positions
          this->moving_platforms.push_back({
            geometry::Position{x * 16, y * 16},
            geometry::Position{x * 16, y * 16},
            true,
          });
          break;
        case 'k':
          sprite = static_cast<int>(Sprite::SPRITE_CONCRETE_V);
          break;
        case 'K':
          sprite = static_cast<int>(Sprite::SPRITE_CONCRETE);
          break;
        case 'l':
          sprite = static_cast<int>(Sprite::SPRITE_CONCRETE_X);
          break;
        case 'L':
          sprite = static_cast<int>(Sprite::SPRITE_CONCRETE_H);
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
          break;
        case 'N':
          // TODO: earth?
          sprite = static_cast<int>(Sprite::SPRITE_EARTH);
          break;
        case 'u':
          // TODO: volcano spawn point?
          break;
        case 'x':
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
              // [m = mine sign
            case 'm':
              tiles.push_back(Tile(static_cast<int>(Sprite::SPRITE_MINE_SIGN_1), 1, 0));
              i++;
              tiles.push_back(Tile(static_cast<int>(Sprite::SPRITE_MINE_SIGN_2), 1, 0));
              continue;
              // [d = danger sign
            case 'd':
              tiles.push_back(Tile(static_cast<int>(Sprite::SPRITE_DANGER_1), 1, 0));
              i++;
              tiles.push_back(Tile(static_cast<int>(Sprite::SPRITE_DANGER_2), 1, 0));
              continue;
            default:
              break;
          }
          break;
        case -6:
          sprite = static_cast<int>(Sprite::SPRITE_BARREL_CRACKED);
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
          break;
        case -79:
          sprite = static_cast<int>(Sprite::SPRITE_WOOD_H);
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
        tile = Tile(sprite, 1, 0);
      }
    }
    else
    {
      // Use object manager to populate tiles
      if (tile_id == -1)
      {
        tile = Tile::INVALID;
      }
      else
      {
        tile = object_manager.get_tile(tile_id);
      }
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
