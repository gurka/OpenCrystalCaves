#include "state.h"

#include <sstream>

#include "constants.h"


State::~State() {}


State* State::update(const Input& input)
{
	(void)input;
	ticks_++;
	return this;
}


PlayerInput input_to_player_input(const Input& input)
{
  PlayerInput pi;
  pi.left = input.left.down;
  pi.right = input.right.down;
  pi.jump = input.z.down;
  pi.shoot = input.x.down;
	pi.leftPressed = input.left.pressed();
	pi.rightPressed = input.right.pressed();
	pi.jumpPressed = input.z.pressed();
	pi.shootPressed = input.x.pressed();
  return pi;
}

State* SkipState::update(const Input& input)
{
	auto next = State::update(input);
	if (ticks_ > grace_ticks_)
	{
		auto pinput = input_to_player_input(input);
		if(pinput.jumpPressed || pinput.shootPressed)
		{
			return next_state_;
		}
	}
	return next;
}

SplashState::SplashState(std::vector<Surface*>& images, Window& window) : images_(images), overlay_(std::unique_ptr<Surface>(Surface::from_pixels(images[0]->width(), images[0]->height(), nullptr, window))) {
	uint32_t pixels = 0xff000000;
	overlay_ = std::unique_ptr<Surface>(Surface::from_pixels(1, 1, &pixels, window));
}

void SplashState::draw(Window& window) const
{
	(void)window;
	images_[0]->blit_surface(geometry::Rectangle(0, 0, images_[0]->size()),
							geometry::Rectangle((WINDOW_SIZE - CAMERA_SIZE_SCALED) / 2, CAMERA_SIZE_SCALED));
	// Draw black shade that becomes more transparent over time
	if (ticks_ < grace_ticks_)
	{
		overlay_->set_alpha((grace_ticks_ - ticks_) * 256 / grace_ticks_);
		overlay_->blit_surface(geometry::Rectangle(0, 0, 1, 1),
							  geometry::Rectangle(0, 0, CAMERA_SIZE_SCALED));
	}
}

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
			image->blit_surface(geometry::Rectangle(0, 0, image->size()),
								geometry::Rectangle(0, y, CAMERA_SIZE_SCALED));
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
			image->blit_surface(geometry::Rectangle(0, 0, image->size()),
								geometry::Rectangle(0, y, CAMERA_SIZE_SCALED));
			y += CAMERA_SIZE_SCALED.y();
		}
	}
}


GameState::GameState(Game& game, SpriteManager& sprite_manager, Surface& game_surface, Window& window) : game_(game), game_surface_(game_surface), game_renderer_(&game, &sprite_manager, &game_surface, window)
{}

State* GameState::update(const Input& input)
{
	auto next = State::update(input);
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
	return next;
}

void render_statusbar(Window& window, unsigned score, unsigned num_ammo, unsigned num_lives)
{
  constexpr auto statusbar_height = WINDOW_SIZE.y() / 12 / 2;
  constexpr auto statusbar_rect = geometry::Rectangle(0, WINDOW_SIZE.y() - statusbar_height,
													  WINDOW_SIZE.x(), statusbar_height);

  char statusbar_text[64];
  snprintf(statusbar_text, 64, "Score: %8u Ammo: %2u Life: %u", score, num_ammo, num_lives);

  window.fill_rect(statusbar_rect, { 0u, 0u, 0u });
  window.render_text(statusbar_rect.position + geometry::Position(32, 0),
					 statusbar_text,
					 24,
					 { 255u, 255u, 255u });
}

void GameState::draw(Window& window) const
{
	// Clear window surface
	window.fill_rect(geometry::Rectangle(0, 0, WINDOW_SIZE), { 33u, 33u, 33u });
	
	// Render game
	game_renderer_.render_game();
	
	// Render game surface to window surface, centered and scaled
	game_surface_.blit_surface(geometry::Rectangle(0, 0, CAMERA_SIZE),
							   geometry::Rectangle((WINDOW_SIZE - CAMERA_SIZE_SCALED) / 2, CAMERA_SIZE_SCALED));

	// Render statusbar
	render_statusbar(window, game_.get_score(), game_.get_num_ammo(), game_.get_num_lives());

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
	  window.fill_rect({ 0, 24, 200, 20 + (20 * static_cast<int>(game_debug_infos.size())) }, { 0u, 0u, 0u });

	  // Render debug text
	  auto pos_y = 25;
	  const auto& game_camera = game_renderer_.get_game_camera();
	  const auto camera_position_str = "camera position: (" + std::to_string(game_camera.position.x()) + ", " + std::to_string(game_camera.position.y()) + ")";
	  window.render_text(geometry::Position(5,  pos_y), camera_position_str, 12, { 255u, 0u, 0u });
	  pos_y += 20;

	  for (const auto& game_debug_info : game_debug_infos)
	  {
		window.render_text(geometry::Position(5,  pos_y), game_debug_info, 12, { 255u, 0u, 0u });
		pos_y += 20;
	  }
	}
}
