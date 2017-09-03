#ifndef LEVEL_H_
#define LEVEL_H_

#include <array>
#include <vector>

#include "item.h"
#include "geometry.h"
#include "player.h"

struct Object
{
  Object(geometry::Position position, geometry::Size size, int sprite_id)
    : position(position),
      size(size),
      sprite_id(sprite_id)
  {
  }

  geometry::Position position;
  geometry::Size size;
  int sprite_id;
};

class Level
{
 public:
  Level();
  Level(int width,
        int height,
        std::vector<Item::Id> tiles_background,
        std::vector<Item::Id> tiles_foreground,
        std::vector<geometry::Rectangle> aabbs,
        std::vector<geometry::Position> platforms);

  bool valid() const { return valid_; }

  int get_tile_width() const { return width_; }
  int get_tile_height() const { return height_; }

  Player& get_player() { return player_; }
  const Player& get_player() const { return player_; }

  Item::Id get_tile_background(int tile_x, int tile_y) const;
  Item::Id get_tile_foreground(int tile_x, int tile_y) const;

  const std::vector<geometry::Rectangle>& get_aabbs() const { return aabbs_; }
  const std::vector<geometry::Position>& get_platforms() const { return platforms_; }

  // Dynamic things in the level
  void update(unsigned game_tick);
  const std::vector<Object>& get_objects() const { return objects_; }
  const std::vector<geometry::Position>& get_moving_platforms() const { return moving_platforms_; }

  // Helpers
  bool collides(const geometry::Rectangle& rect);

 private:
  Item::Id get_tile(int tile_x, int tile_y, const std::vector<Item::Id>& items) const;

  // false if Level is default constructed
  bool valid_;

  // Size of level in number of tiles
  int width_;
  int height_;

  // The player
  Player player_;

  // Static tiles, in order
  std::vector<Item::Id> tiles_background_;
  std::vector<Item::Id> tiles_foreground_;

  // Bounding boxes for collision
  std::vector<geometry::Rectangle> aabbs_;
  std::vector<geometry::Position> platforms_;  // FIXME: Rectangle?

  // Dynamic objects
  std::vector<Object> objects_;  // Used for rendering
  std::vector<geometry::Position> moving_platforms_;  // Used for collision

  // TODO: These belong to mainlevel.json only. Decide how to fix.
  struct
  {
    bool down = true;
    int position_y = 7 * 16;  // normal pixel
  } vertical_platform_;

  struct
  {
    bool right = true;
    int position_x = 7 * 16;  // normal pixel
  } horizontal_platform_;

  struct
  {
    bool right = false;  // true -> going right, false -> going left
    int position_x = 128;  // subpixel: earth.position.x() = position_x / 2;
  } earth_;

  struct
  {
    bool right = false;  // true -> going right, false -> going left
    int position_x = 160;  // subpixel: moon.position.x() = position_x / 2;
  } moon_;

  struct
  {
    // 4 different sprites (times 2 for left/right)
    // Changes sprite 27 times (4 + 4 + 4 + 4 + 4 + 4 + 3)
    // Each sprite visible for 3 ticks
    // Visible for 27 * 3 = 81 ticks
    // Visible for 4630ms = 81 ticks
    // Idle for 12567ms = 220 ticks

    // Sprite ids:
    // Left  Right
    //  752   748
    //  753   749
    //  754   750
    //  755   751
    bool active = false;
    unsigned tick_start = 0u;  // tick when active changed
  } volcano_;
};

#endif  // LEVEL_H_
