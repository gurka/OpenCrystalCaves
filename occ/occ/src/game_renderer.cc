#include "game_renderer.h"

#include "constants.h"
#include "game.h"
#include "graphics.h"
#include "level.h"
#include "misc.h"
#include "occ_math.h"
#include "player.h"
#include "spritemgr.h"

GameRenderer::GameRenderer(Game* game, SpriteManager* sprite_manager, Surface* game_surface, Window& window)
  : game_(game),
    sprite_manager_(sprite_manager),
    game_surface_(game_surface),
    window_(window),
    game_camera_(math::clamp(game_->get_player().position.x() + (game_->get_player().size.x() / 2) - (CAMERA_SIZE.x() / 2),
                             0,
                             (game_->get_tile_width() * 16) - CAMERA_SIZE.x()),
                 math::clamp(game_->get_player().position.y() + (game_->get_player().size.y() / 2) - (CAMERA_SIZE.y() / 2),
                             0,
                             (game_->get_tile_height() * 16) - CAMERA_SIZE.y()),
                 CAMERA_SIZE.x(),
                 CAMERA_SIZE.y()),
    game_tick_(0u),
    game_tick_diff_(0u),
    debug_(false)
{
}

void GameRenderer::update(unsigned game_tick)
{
  game_tick_diff_ = game_tick - game_tick_;
  game_tick_ = game_tick;

  // Update game camera
  // Note: this isn't exactly how the Crystal Caves camera work, but it's good enough
  const geometry::Position player_camera_relative_position{(game_->get_player().position + (game_->get_player().size / 2)) -
                                                           (game_camera_.position + (game_camera_.size / 2))};
  if (player_camera_relative_position.x() < -4)
  {
    game_camera_.position = geometry::Position(
      math::clamp(game_camera_.position.x() + player_camera_relative_position.x() + 4, 0, (game_->get_tile_width() * 16) - CAMERA_SIZE.x()),
      game_camera_.position.y());
  }
  else if (player_camera_relative_position.x() > 20)
  {
    game_camera_.position = geometry::Position(math::clamp(game_camera_.position.x() + player_camera_relative_position.x() - 20,
                                                           0,
                                                           (game_->get_tile_width() * 16) - CAMERA_SIZE.x()),
                                               game_camera_.position.y());
  }

  if (player_camera_relative_position.y() < -10)
  {
    game_camera_.position = geometry::Position(game_camera_.position.x(),
                                               math::clamp(game_camera_.position.y() + player_camera_relative_position.y() + 10,
                                                           0,
                                                           (game_->get_tile_height() * 16) - CAMERA_SIZE.y()));
  }
  else if (player_camera_relative_position.y() > 32)
  {
    game_camera_.position = geometry::Position(game_camera_.position.x(),
                                               math::clamp(game_camera_.position.y() + player_camera_relative_position.y() - 32,
                                                           0,
                                                           (game_->get_tile_height() * 16) - CAMERA_SIZE.y()));
  }
}

void GameRenderer::render_game() const
{
  window_.set_render_target(game_surface_);
  // Clear game surface (background now)
  window_.fill_rect(geometry::Rectangle(0, 0, CAMERA_SIZE), {33u, 33u, 33u});
  render_background();
  render_tiles(false);
  render_objects();
  render_player();
  render_enemies();
  render_tiles(true);
  render_items();
  render_statusbar();
  window_.set_render_target(nullptr);
}

void GameRenderer::render_background() const
{
  // TODO: Create a surface of size CAMERA + (background.size() * 16) and render the background
  //       to it _once_, then just keep render that surface (with game_camera offset) until the
  //       level changes.

  const auto start_tile_x = game_camera_.position.x() > 0 ? game_camera_.position.x() / 16 : 0;
  const auto start_tile_y = game_camera_.position.y() > 0 ? game_camera_.position.y() / 16 : 0;
  const auto end_tile_x = (game_camera_.position.x() + game_camera_.size.x()) / 16;
  const auto end_tile_y = (game_camera_.position.y() + game_camera_.size.y()) / 16;

  for (int tile_y = start_tile_y; tile_y <= end_tile_y; tile_y++)
  {
    for (int tile_x = start_tile_x; tile_x <= end_tile_x; tile_x++)
    {
      const auto sprite_id = game_->get_bg_sprite(tile_x, tile_y);
      if (sprite_id != -1)
      {
        sprite_manager_->render_tile(sprite_id, {tile_x * SPRITE_W, tile_y * SPRITE_H}, game_camera_.position);
      }
    }
  }

  // MAIN_LEVEL has some special things that needs to be rendered
  if (game_->get_level_id() == LevelId::MAIN_LEVEL)
  {
    // Might be cleaner to have this in a dedicated struct for MainLevel stuff

    static bool volcano_active = false;
    static unsigned volcano_tick_start = 0u;

    static bool earth_right = false;
    static int earth_pos_x = 128;

    static bool moon_right = false;
    static int moon_pos_x = 160;

    // Update earth and moon, but only when game tick has increased since they use absolute movement
    if (game_tick_diff_ != 0)
    {
      if (earth_right)
      {
        earth_pos_x += 1;
        if ((earth_pos_x / 2) + 16 == game_->get_tile_width() * 16)
        {
          earth_right = false;
        }
      }
      else
      {
        earth_pos_x -= 1;
        if (earth_pos_x == 32)
        {
          earth_right = true;
        }
      }

      if (moon_right)
      {
        moon_pos_x += earth_right ? 2 : 1;
        if (moon_pos_x > earth_pos_x + 72 || (moon_pos_x / 2) + 16 == game_->get_tile_width() * 16)
        {
          moon_right = false;
        }
      }
      else
      {
        moon_pos_x -= !earth_right ? 2 : 1;
        if (moon_pos_x < earth_pos_x - 64 || moon_pos_x == 32)
        {
          moon_right = true;
        }
      }
    }

    // Update volcano
    if (volcano_active && game_tick_ - volcano_tick_start >= 81u)
    {
      volcano_active = false;
      volcano_tick_start = game_tick_;
    }
    else if (!volcano_active && game_tick_ - volcano_tick_start >= 220u)
    {
      volcano_active = true;
      volcano_tick_start = game_tick_;
    }

    // Render earth and moon if visible
    const auto earth_rect = geometry::Rectangle(geometry::Position(earth_pos_x / 2, 0), geometry::Size(16, 16));
    const auto moon_rect = geometry::Rectangle(geometry::Position(moon_pos_x / 2, 0), geometry::Size(16, 16));
    if (moon_right)
    {
      // Moon is behind earth, render moon first
      if (geometry::isColliding(game_camera_, moon_rect))
      {
        sprite_manager_->render_tile(static_cast<int>(Sprite::SPRITE_MOON), moon_rect.position, game_camera_.position);
      }
      if (geometry::isColliding(game_camera_, earth_rect))
      {
        sprite_manager_->render_tile(static_cast<int>(Sprite::SPRITE_EARTH), earth_rect.position, game_camera_.position);
      }
    }
    else
    {
      // Earth is behind moon, render earth first
      if (geometry::isColliding(game_camera_, earth_rect))
      {
        sprite_manager_->render_tile(static_cast<int>(Sprite::SPRITE_EARTH), earth_rect.position, game_camera_.position);
      }
      if (geometry::isColliding(game_camera_, moon_rect))
      {
        sprite_manager_->render_tile(static_cast<int>(Sprite::SPRITE_MOON), moon_rect.position, game_camera_.position);
      }
    }

    // Render volcano fire if active and visible
    if (volcano_active)
    {
      if (start_tile_x <= 29 && end_tile_x >= 29)
      {
        const auto sprite_id = 752 + ((game_tick_ - volcano_tick_start) / 3) % 4;
        sprite_manager_->render_tile(sprite_id, {29 * SPRITE_W, 2 * SPRITE_H}, game_camera_.position);
      }
      if (start_tile_x <= 30 && end_tile_x >= 30)
      {
        const auto sprite_id = 748 + ((game_tick_ - volcano_tick_start) / 3) % 4;
        sprite_manager_->render_tile(sprite_id, {30 * SPRITE_W, 2 * SPRITE_H}, game_camera_.position);
      }
    }
  }
}

void GameRenderer::render_player() const
{
  // Player sprite ids
  static constexpr std::array<int, 12> sprite_walking_right = {260, 261, 262, 263, 264, 265, 266, 267, 268, 269, 270, 271};
  static constexpr int sprite_jumping_right = 284;
  static constexpr int sprite_shooting_right = 286;

  static constexpr int sprite_standing_left = 272;
  static constexpr std::array<int, 12> sprite_walking_left = {272, 273, 274, 275, 276, 277, 278, 279, 280, 281, 282, 283};
  static constexpr int sprite_jumping_left = 285;
  static constexpr int sprite_shooting_left = 287;

  const geometry::Rectangle src_rect = [this]()
  {
    const auto& player = game_->get_player();
    int sprite = 0;

    // Sprite selection priority: (currently 'shooting' means pressing shoot button without ammo)
    // If walking:
    //   1. Jumping or falling
    //   2. Walking
    // Else:
    //   1. Shooting
    //   2. Jumping or falling
    //   3. Standing still

    if (player.direction == Player::Direction::right)
    {
      if (player.walking)
      {
        if (player.jumping || player.falling)
        {
          sprite = sprite_jumping_right;
        }
        else
        {
          sprite = sprite_walking_right[player.walk_tick % sprite_walking_right.size()];
        }
      }
      else
      {
        if (player.shooting)
        {
          sprite = sprite_shooting_right;
        }
        else if (player.jumping || player.falling)
        {
          sprite = sprite_jumping_right;
        }
        else
        {
          sprite = static_cast<int>(Sprite::SPRITE_STANDING_RIGHT);
        }
      }
    }
    else  // player_.direction == Player::Direction::left
    {
      if (player.walking)
      {
        if (player.jumping || player.falling)
        {
          sprite = sprite_jumping_left;
        }
        else
        {
          sprite = sprite_walking_left[player.walk_tick % sprite_walking_left.size()];
        }
      }
      else
      {
        if (player.shooting)
        {
          sprite = sprite_shooting_left;
        }
        else if (player.jumping || player.falling)
        {
          sprite = sprite_jumping_left;
        }
        else
        {
          sprite = sprite_standing_left;
        }
      }
    }
    if (player.reverse_gravity)
    {
      sprite += 104;
    }
    return sprite_manager_->get_rect_for_tile(sprite);
  }();
  const auto player_render_pos = game_->get_player().position - game_camera_.position;

  // Note: player size is 12x16 but the sprite is 16x16 so we need to adjust where
  // the player is rendered
  const geometry::Rectangle dest_rect{player_render_pos.x() - 2, player_render_pos.y(), 16, 16};

  sprite_manager_->get_surface()->blit_surface(src_rect, dest_rect);

  if (debug_)
  {
    window_.render_rectangle(dest_rect, {255, 0, 0});
  }
}

void GameRenderer::render_enemies() const
{
  for (const auto& enemy : game_->get_enemies())
  {
    static constexpr geometry::Size object_size = geometry::Size(16, 16);
    if (geometry::isColliding(geometry::Rectangle(enemy.position, object_size), game_camera_))
    {
      const auto sprite_id = enemy.sprites[game_tick_ % enemy.sprites.size()];
      sprite_manager_->render_tile(sprite_id, enemy.position, game_camera_.position);

      if (debug_)
      {
        const geometry::Rectangle dest_rect{
          enemy.position.x() - game_camera_.position.x(), enemy.position.y() - game_camera_.position.y(), object_size.x(), object_size.y()};
        window_.render_rectangle(dest_rect, {255, 0, 0});
      }
    }
  }
}

void GameRenderer::render_tiles(bool in_front) const
{
  const auto start_tile_x = game_camera_.position.x() > 0 ? game_camera_.position.x() / 16 : 0;
  const auto start_tile_y = game_camera_.position.y() > 0 ? game_camera_.position.y() / 16 : 0;
  const auto end_tile_x = (game_camera_.position.x() + game_camera_.size.x()) / 16;
  const auto end_tile_y = (game_camera_.position.y() + game_camera_.size.y()) / 16;

  for (int tile_y = start_tile_y; tile_y <= end_tile_y; tile_y++)
  {
    for (int tile_x = start_tile_x; tile_x <= end_tile_x; tile_x++)
    {
      const auto& tile = game_->get_tile(tile_x, tile_y);

      if (!tile.valid())
      {
        continue;
      }

      if ((in_front && !tile.is_render_in_front()) || (!in_front && tile.is_render_in_front()))
      {
        continue;
      }

      const auto sprite_id = [&tile](unsigned game_tick)
      {
        if (tile.is_animated())
        {
          return tile.get_sprite() + static_cast<int>((game_tick / 2) % tile.get_sprite_count());
        }
        else
        {
          return tile.get_sprite();
        }
      }(game_tick_);
      sprite_manager_->render_tile(sprite_id, {tile_x * SPRITE_W, tile_y * SPRITE_H}, game_camera_.position);
    }
  }
}

void GameRenderer::render_objects() const
{
  for (const auto& object : game_->get_objects())
  {
    static constexpr geometry::Size object_size = geometry::Size(16, 16);
    if (geometry::isColliding(geometry::Rectangle(object.position, object_size), game_camera_))
    {
      const auto sprite_id = object.get_sprite(game_tick_);
      sprite_manager_->render_tile(sprite_id, object.position, game_camera_.position);

      if (debug_)
      {
        const geometry::Rectangle dest_rect{object.position.x() - game_camera_.position.x(),
                                            object.position.y() - game_camera_.position.y(),
                                            object_size.x(),
                                            object_size.y()};
        window_.render_rectangle(dest_rect, {255, 0, 0});
      }
    }
  }
}

void GameRenderer::render_items() const
{
  const auto start_tile_x = game_camera_.position.x() > 0 ? game_camera_.position.x() / 16 : 0;
  const auto start_tile_y = game_camera_.position.y() > 0 ? game_camera_.position.y() / 16 : 0;
  const auto end_tile_x = (game_camera_.position.x() + game_camera_.size.x()) / 16;
  const auto end_tile_y = (game_camera_.position.y() + game_camera_.size.y()) / 16;

  for (int tile_y = start_tile_y; tile_y <= end_tile_y; tile_y++)
  {
    for (int tile_x = start_tile_x; tile_x <= end_tile_x; tile_x++)
    {
      const auto& item = game_->get_item(tile_x, tile_y);

      if (!item.valid())
      {
        continue;
      }

      sprite_manager_->render_tile(item.get_sprite(), {tile_x * SPRITE_W, tile_y * SPRITE_H}, game_camera_.position);
    }
  }
}

void GameRenderer::render_statusbar() const
{
  constexpr auto statusbar_height = CHAR_H;
  const auto statusbar_rect = geometry::Rectangle(0, game_camera_.size.y() - CHAR_H, game_camera_.size.x(), statusbar_height);

  window_.fill_rect(statusbar_rect, {0u, 0u, 0u});

  constexpr int dy = 1;
  // $
  sprite_manager_->render_text(L"$", statusbar_rect.position + geometry::Position(0, dy));
  // score
  sprite_manager_->render_number(game_->get_score(), statusbar_rect.position + geometry::Position(8 * CHAR_W, dy));
  // Gun
  sprite_manager_->render_icon(Icon::ICON_GUN, statusbar_rect.position + geometry::Position(11 * CHAR_W, dy));
  // ammo
  sprite_manager_->render_number(game_->get_num_ammo(), statusbar_rect.position + geometry::Position(15 * CHAR_W, dy));
  // Hearts
  for (unsigned i = 0; i < game_->get_num_lives(); i++)
  {
    sprite_manager_->render_icon(Icon::ICON_HEART, statusbar_rect.position + geometry::Position((i + 19) * CHAR_W, dy));
  }
  // Key
  if (game_->has_key())
  {
    sprite_manager_->render_icon(Icon::ICON_KEY, statusbar_rect.position + geometry::Position(23 * CHAR_W, dy));
  }
}
