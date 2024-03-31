#include "state.h"

#include <sstream>

#include <easing.h>

#include "constants.h"
#include "utils.h"
#include <path.h>

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


void SkipState::update(const Input& input)
{
  State::update(input);
  if (ticks_ > grace_ticks_)
  {
    const auto pinput = input_to_player_input(input);
    if (pinput.jump_pressed || pinput.shoot_pressed || input.escape.pressed())
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

#ifdef _MSC_VER
#define OS_NAME "Windows"
#elif defined(__APPLE__)
#define OS_NAME "macOS"
#else
#define OS_NAME "Linux"
#endif

TitleState::TitleState(SpriteManager& sprite_manager,
                       Surface& game_surface,
                       std::vector<Surface*>& images,
                       Window& window,
                       ExeData& exe_data)
  : State(FADE_TICKS, FADE_TICKS, window),
    sprite_manager_(sprite_manager),
    game_surface_(game_surface),
    images_(images),
    panel_(
      {
        L"Welcome to OpenCrystalCaves!",
        L"----------------------------",
        L"     New Game",
        L"     Restore Game",
        L"     Ordering Info.",  // TODO: hide if retail version detected
        L"     Instructions",
        L"     Story",
        L"     High Scores",
        L"     Visit our Website",
        L"     About Apogee",
        L"     Quit to " OS_NAME,
      },
      {
        {2, {PanelType::PANEL_TYPE_NEW_GAME}},
        {3, {PanelType::PANEL_TYPE_DISABLED}},
        // Check if we have the registered version (randomly load a episode 2 file)
        // If so disable the ordering instructions panel
        {4, get_data_path("CC2.APG").empty() ? Panel(PanelText::PANEL_TEXT_ORDER_3, exe_data) : Panel(PanelType::PANEL_TYPE_DISABLED)},
        {
          5,
          {{{PanelText::PANEL_TEXT_INSTRUCTIONS_1, exe_data},
            {PanelText::PANEL_TEXT_INSTRUCTIONS_2, exe_data},
            {PanelText::PANEL_TEXT_INSTRUCTIONS_3, exe_data},
            {PanelText::PANEL_TEXT_INSTRUCTIONS_4, exe_data},
            {PanelText::PANEL_TEXT_INSTRUCTIONS_5, exe_data},
            {PanelText::PANEL_TEXT_INSTRUCTIONS_6, exe_data},
            {PanelText::PANEL_TEXT_INSTRUCTIONS_7, exe_data}}},
        },
        {6, {PanelType::PANEL_TYPE_DISABLED}},
        {7, {PanelType::PANEL_TYPE_DISABLED}},
        {8, {PanelType::PANEL_TYPE_DISABLED}},
        {9, {PanelText::PANEL_TEXT_ABOUT, exe_data}},
        {10, {PanelType::PANEL_TYPE_QUIT_TO_OS}},
      })
{
}

void TitleState::update(const Input& input)
{
  State::update(input);
  const auto pinput = input_to_player_input(input);

  if (panel_current_ == nullptr)
  {
    if (pinput.jump_pressed || pinput.shoot_pressed || input.escape.pressed())
    {
      panel_current_ = &panel_;
    }
  }
  else if (input.escape.pressed())
  {
    panel_current_ = nullptr;
  }
  else
  {
    panel_current_ = panel_current_->update(input);
  }
  if (panel_current_)
  {
    switch (panel_current_->get_type())
    {
      case PanelType::PANEL_TYPE_NEW_GAME:
        finish();
        break;
      case PanelType::PANEL_TYPE_QUIT_TO_OS:
        finish();
        break;
      default:
        break;
        // TODO: handle other types
    }
  }
  else
  {
    // Don't scroll background if panel shown
    scroll_ticks_ += 1;
  }
}

void TitleState::draw(Window& window) const
{
  constexpr unsigned first_ticks = 50;
  constexpr unsigned scroll_ticks = 50;
  constexpr unsigned last_ticks = 50;
  const auto period_ticks = first_ticks + scroll_ticks * 2 + last_ticks;
  const auto ticks = scroll_ticks_ % period_ticks;
  if (ticks < first_ticks)
  {
    // Show first image
    images_[0]->blit_surface(geometry::Rectangle(0, 0, images_[0]->size()),
                             geometry::Rectangle((WINDOW_SIZE - CAMERA_SIZE_SCALED) / 2, CAMERA_SIZE_SCALED));
  }
  else if (ticks < first_ticks + scroll_ticks)
  {
    // Show scrolling from first image to last
    const auto scroll = (float)(ticks - first_ticks) / scroll_ticks * CAMERA_SIZE_SCALED.y();
    auto y = -(int)scroll;
    for (auto& image : images_)
    {
      image->blit_surface(geometry::Rectangle(0, 0, image->size()), geometry::Rectangle(0, y, CAMERA_SIZE_SCALED));
      y += CAMERA_SIZE_SCALED.y();
    }
  }
  else if (ticks < first_ticks + scroll_ticks + last_ticks)
  {
    // Show last image
    images_.back()->blit_surface(geometry::Rectangle(0, 0, images_.back()->size()),
                                 geometry::Rectangle((WINDOW_SIZE - CAMERA_SIZE_SCALED) / 2, CAMERA_SIZE_SCALED));
  }
  else
  {
    // Show scrolling from last image to first
    const auto scroll = (1 - (float)(ticks - first_ticks - scroll_ticks - last_ticks) / scroll_ticks) * CAMERA_SIZE_SCALED.y();
    auto y = -(int)scroll;
    for (auto& image : images_)
    {
      image->blit_surface(geometry::Rectangle(0, 0, image->size()), geometry::Rectangle(0, y, CAMERA_SIZE_SCALED));
      y += CAMERA_SIZE_SCALED.y();
    }
  }
  if (panel_current_)
  {
    // Blit to game surface to set scaling properly
    window.set_render_target(&game_surface_);
    // Clear window surface
    window.fill_rect(geometry::Rectangle(0, 0, WINDOW_SIZE), {33u, 33u, 33u, 0u});

    panel_current_->draw(sprite_manager_);
    window.set_render_target(nullptr);
    // Render game surface to window surface, centered and scaled
    game_surface_.blit_surface(geometry::Rectangle(0, 0, CAMERA_SIZE),
                               geometry::Rectangle((WINDOW_SIZE - CAMERA_SIZE_SCALED) / 2, CAMERA_SIZE_SCALED));
  }
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
  if (input.escape.pressed())
  {
    finish();
  }
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
    auto game_debug_info_iss = std::wistringstream(game_.get_debug_info());
    std::wstring temp;
    std::vector<std::wstring> game_debug_infos;
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
      L"camera position: (" + std::to_wstring(game_camera.position.x()) + L", " + std::to_wstring(game_camera.position.y()) + L")";
    sprite_manager_.render_text(camera_position_str, geometry::Position(5, pos_y));
    pos_y += 20;

    for (const auto& game_debug_info : game_debug_infos)
    {
      sprite_manager_.render_text(game_debug_info, geometry::Position(5, pos_y));
      pos_y += 20;
    }
  }
}
