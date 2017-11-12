#ifndef GAME_RENDERER_H_
#define GAME_RENDERER_H_

#include "geometry.h"

class Game;
class SpriteManager;
class Surface;

class GameRenderer
{
 public:
  GameRenderer(Game* game, SpriteManager* sprite_manager, Surface* game_surface);

  void render_game(unsigned game_tick);

  const geometry::Rectangle& get_game_camera() const { return game_camera_; }

  bool get_debug() const { return debug_; }
  void set_debug(bool debug) { debug_ = debug; }

 private:
  void render_background();
  void render_player();
  void render_enemies();
  void render_tiles(bool in_front);
  void render_objects();
  void render_items();

  Game* game_;
  SpriteManager* sprite_manager_;
  Surface* game_surface_;

  geometry::Rectangle game_camera_;

  unsigned game_tick_;
  unsigned game_tick_diff_;

  bool debug_;
};

#endif  // GAME_RENDERER_H_
