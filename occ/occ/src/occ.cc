#include <cassert>
#include <cstdio>
#include <cstdlib>

#include <sstream>
#include <memory>
#include <utility>

#include "constants.h"
#include "game_renderer.h"
#include "spritemgr.h"

// From game
#include "game.h"
#include "player_input.h"
#include "player.h"

// From sge
#include "sge.h"
#include "graphics.h"
#include "input.h"

// From sge_utils
#include "geometry.h"
#include "logger.h"

PlayerInput input_to_player_input(const Input& input)
{
  PlayerInput pi;
  pi.left = input.left.pressed;
  pi.right = input.right.pressed;
  pi.jump = input.z.pressed;
  pi.shoot = input.x.pressed;
  return pi;
}

void render_statusbar(Surface* surface, unsigned score, unsigned num_ammo, unsigned num_lives)
{
  constexpr auto statusbar_height = WINDOW_SIZE.y() / 12 / 2;
  constexpr auto statusbar_rect = geometry::Rectangle(0, WINDOW_SIZE.y() - statusbar_height,
                                                      WINDOW_SIZE.x(), statusbar_height);

  char statusbar_text[64];
  snprintf(statusbar_text, 64, "Score: %8u Ammo: %2u Life: %u", score, num_ammo, num_lives);

  surface->fill_rect(statusbar_rect, { 0u, 0u, 0u });
  surface->render_text(statusbar_rect.position + geometry::Position(32, 0),
                       statusbar_text,
                       24,
                       { 255u, 255u, 255u });
}

int main()
{
  LOG_INFO("Starting!");

  // Init SGEngine
  auto engine = SGEngine::create("OpenCrystalCaves", WINDOW_SIZE);
  if (!engine)
  {
    LOG_CRITICAL("Could not create SGEngine");
    return 1;
  }

  auto window_surface = engine->get_window_surface();
  LOG_INFO("Window created");

  // Create game surface
  std::unique_ptr<Surface> game_surface = engine->create_surface(CAMERA_SIZE);
  if (!game_surface)
  {
    LOG_CRITICAL("Could not create game surface");
    return 1;
  }
  LOG_INFO("Game surface created");

  // Load tileset
  SpriteManager sprite_manager;
  if (!sprite_manager.load_tileset("media/sprites.bmp"))
  {
    LOG_CRITICAL("Could not load tileset");
    return 1;
  }
  LOG_INFO("Tileset loaded");

  // Create Game
  std::unique_ptr<Game> game = Game::create();
  if (!game)
  {
    LOG_CRITICAL("Could not create Game");
    return 1;
  }
  if (!game->init())
  {
    LOG_CRITICAL("Could not initialize Game");
    return 1;
  }
  LOG_INFO("Game initialized");

  // Create GameRenderer
  GameRenderer game_renderer(game.get(), &sprite_manager, game_surface.get());

  // Create game tick function
  auto game_tick_func = [&game](SGEngineSettings* settings, unsigned engine_tick, const Input& input)
  {
    if (input.quit)
    {
      settings->quit();
    }
    else if (input.num_1.pressed && !input.num_1.repeated)
    {
      settings->toggle_show_fps();
    }

    game->update(engine_tick, input_to_player_input(input));
  };

  // Create render function
  auto render_func = [&window_surface, &game_renderer, &game_surface, &game](unsigned engine_tick)
  {
    // Clear window surface
    window_surface->fill_rect(geometry::Rectangle(0, 0, WINDOW_SIZE), { 33u, 33u, 33u });

    // Render game
    game_renderer.render_game(engine_tick);

    // Render game surface to window surface, centered and scaled
    window_surface->blit_surface(game_surface.get(),
                                 geometry::Rectangle(0, 0, CAMERA_SIZE),
                                 geometry::Rectangle((WINDOW_SIZE - CAMERA_SIZE_SCALED) / 2, CAMERA_SIZE_SCALED),
                                 BlitType::SCALE);

    // Render statusbar
    render_statusbar(window_surface.get(), game->get_score(), game->get_num_ammo(), game->get_num_lives());
  };

  // Run engine
  engine->run(game_tick_func, render_func);

  return 0;
}
