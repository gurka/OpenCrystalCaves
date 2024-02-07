#include "state.h"

#include <sstream>

#include <easing.h>

#include "constants.h"

static constexpr int FADE_TICKS = 15;

State::State(const unsigned fade_in_ticks, const unsigned fade_out_ticks, Window& window)
  : overlay_(std::unique_ptr<Surface>(Surface::from_pixels(SCREEN_SIZE.x(), SCREEN_SIZE.y(), nullptr, window))),
    fade_in_ticks_(fade_in_ticks),
    fade_out_ticks_(fade_out_ticks)
{
  uint32_t pixels = 0xff000000;
  overlay_ = std::unique_ptr<Surface>(Surface::from_pixels(1, 1, &pixels, window));
}


void State::update(const Input& input)
{
  (void)input;
  ticks_++;
}

void State::draw(Window& window) const
{
  (void)window;
  // Draw black shade overlay
  float dd = 0;
  if (ticks_ < fade_in_ticks_)
  {
    const auto d = (float)ticks_ / fade_in_ticks_;
    dd = 1.0f - (float)ExponentialEaseIn(d);
  }
  else if (fade_out_start_ticks_ > 0)
  {
    const auto d = (float)(ticks_ - fade_out_start_ticks_) / fade_out_ticks_;
    dd = (float)ExponentialEaseIn((double)d);
  }
  if (dd > 0)
  {
    overlay_->set_alpha((uint8_t)(dd * 255));
    overlay_->blit_surface(geometry::Rectangle(0, 0, 1, 1), geometry::Rectangle(0, 0, CAMERA_SIZE_SCALED));
  }
}


PlayerInput input_to_player_input(const Input& input)
{
  PlayerInput pi;
  pi.left = input.left.down;
  pi.right = input.right.down;
  pi.up = input.up.down;
  pi.down = input.down.down;
  pi.jump = input.z.down;
  pi.shoot = input.x.down;
  pi.left_pressed = input.left.pressed();
  pi.right_pressed = input.right.pressed();
  pi.jump_pressed = input.z.pressed();
  pi.shoot_pressed = input.x.pressed();
  pi.noclip_pressed = input.noclip.pressed();
  pi.ammo_pressed = input.ammo.pressed();
  pi.godmode_pressed = input.godmode.pressed();
  pi.reverse_gravity_pressed = input.reverse_gravity.pressed();
  return pi;
}

void SkipState::update(const Input& input)
{
  State::update(input);
  if (ticks_ > grace_ticks_)
  {
    auto pinput = input_to_player_input(input);
    if (pinput.jump_pressed || pinput.shoot_pressed)
    {
      finish();
    }
  }
}

SplashState::SplashState(std::vector<Surface*>& images, Window& window) : State(FADE_TICKS, 0, window), images_(images) {}

void SplashState::draw(Window& window) const
{
  images_[0]->blit_surface(geometry::Rectangle(0, 0, images_[0]->size()),
                           geometry::Rectangle((WINDOW_SIZE - CAMERA_SIZE_SCALED) / 2, CAMERA_SIZE_SCALED));
  State::draw(window);
}

TitleState::TitleState(std::vector<Surface*>& images, Window& window) : State(FADE_TICKS, FADE_TICKS, window), images_(images) {}

void TitleState::draw(Window& window) const
{
  (void)window;
  const auto period_ticks = first_ticks_ + scroll_ticks_ * 2 + last_ticks_;
  const auto ticks = ticks_ % period_ticks;
  if (ticks < first_ticks_)
  {
    // Show first image
    images_[0]->blit_surface(geometry::Rectangle(0, 0, images_[0]->size()),
                             geometry::Rectangle((WINDOW_SIZE - CAMERA_SIZE_SCALED) / 2, CAMERA_SIZE_SCALED));
  }
  else if (ticks < first_ticks_ + scroll_ticks_)
  {
    // Show scrolling from first image to last
    const auto scroll = (float)(ticks - first_ticks_) / scroll_ticks_ * CAMERA_SIZE_SCALED.y();
    auto y = -(int)scroll;
    for (auto& image : images_)
    {
      image->blit_surface(geometry::Rectangle(0, 0, image->size()), geometry::Rectangle(0, y, CAMERA_SIZE_SCALED));
      y += CAMERA_SIZE_SCALED.y();
    }
  }
  else if (ticks < first_ticks_ + scroll_ticks_ + last_ticks_)
  {
    // Show last image
    images_.back()->blit_surface(geometry::Rectangle(0, 0, images_.back()->size()),
                                 geometry::Rectangle((WINDOW_SIZE - CAMERA_SIZE_SCALED) / 2, CAMERA_SIZE_SCALED));
  }
  else
  {
    // Show scrolling from last image to first
    const auto scroll = (1 - (float)(ticks - first_ticks_ - scroll_ticks_ - last_ticks_) / scroll_ticks_) * CAMERA_SIZE_SCALED.y();
    auto y = -(int)scroll;
    for (auto& image : images_)
    {
      image->blit_surface(geometry::Rectangle(0, 0, image->size()), geometry::Rectangle(0, y, CAMERA_SIZE_SCALED));
      y += CAMERA_SIZE_SCALED.y();
    }
  }
  State::draw(window);
}


GameState::GameState(Game& game, SpriteManager& sprite_manager, Surface& game_surface, Window& window)
  : State(FADE_TICKS, FADE_TICKS, window),
    game_(game),
    game_surface_(game_surface),
	sprite_manager_(sprite_manager),
    game_renderer_(&game, &sprite_manager, &game_surface, window)
{
}

void GameState::update(const Input& input)
{
  State::update(input);
  if (input.num_1.pressed())
  {
    debug_info_ = !debug_info_;
  }
  if (input.num_2.pressed())
  {
    game_renderer_.set_debug(!game_renderer_.get_debug());
  }
  if (input.enter.pressed())
  {
    paused_ = !paused_;
  }

  if (!paused_ || (paused_ && input.space.pressed()))
  {
    // Call game loop
    game_.update(game_tick_, input_to_player_input(input));
    game_tick_ += 1;
  }
  game_renderer_.update(game_tick_);
}

void GameState::draw(Window& window) const
{
  // Clear window surface
  window.fill_rect(geometry::Rectangle(0, 0, WINDOW_SIZE), {33u, 33u, 33u});

  // Render game
  game_renderer_.render_game();

  // Render game surface to window surface, centered and scaled
  game_surface_.blit_surface(geometry::Rectangle(0, 0, CAMERA_SIZE),
                             geometry::Rectangle((WINDOW_SIZE - CAMERA_SIZE_SCALED) / 2, CAMERA_SIZE_SCALED));

  // Debug information
  if (debug_info_)
  {
    // Get debug information from Game and split on newline
    auto game_debug_info_iss = std::istringstream(game_.get_debug_info());
    std::string temp;
    std::vector<std::string> game_debug_infos;
    while (std::getline(game_debug_info_iss, temp))
    {
      game_debug_infos.push_back(std::move(temp));
    }

    // Put a black box where we're going to the draw the debug text
    // 20 pixels per line (1 line + Game's lines)
    window.fill_rect({0, 24, 200, 20 + (20 * static_cast<int>(game_debug_infos.size()))}, {0u, 0u, 0u});

    // Render debug text
    auto pos_y = 25;
    const auto& game_camera = game_renderer_.get_game_camera();
    const auto camera_position_str =
      "camera position: (" + std::to_string(game_camera.position.x()) + ", " + std::to_string(game_camera.position.y()) + ")";
    window.render_text(geometry::Position(5, pos_y), camera_position_str, 12, {255u, 0u, 0u});
    pos_y += 20;

    for (const auto& game_debug_info : game_debug_infos)
    {
      window.render_text(geometry::Position(5, pos_y), game_debug_info, 12, {255u, 0u, 0u});
      pos_y += 20;
    }
  }
}