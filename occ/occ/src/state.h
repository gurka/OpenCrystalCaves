#pragma once

#include "sdl_wrapper.h"
#include "event.h"
#include "graphics.h"
#include "spritemgr.h"
#include "game_renderer.h"

#include "game.h"

/// Represents a game state (e.g. splash, title, game)
/// Contains base logic for fading in/out
class State
{
 public:
	State(const unsigned fade_in_ticks, const unsigned fade_out_ticks, Window& window);
	virtual ~State() = 0;

	void set_next(State& next) { next_state_ = &next; }

	// Called whenever this state is entered
	virtual void reset() {
		ticks_ = 0;
		fade_out_start_ticks_ = 0;
	}

	virtual void update(const Input& input);

	// Called whenever state should start finishing
	void finish() {
		if (fade_out_start_ticks_ == 0) {
			fade_out_start_ticks_ = ticks_;
		}
	}
	bool has_finished() const {
		return fade_out_start_ticks_ > 0 &&
		fade_out_start_ticks_ + fade_out_ticks_ < ticks_;
	}
	State* next_state() {
		return has_finished() ? next_state_ : this;
	}

	virtual void draw(Window& window) const = 0;

protected:
	unsigned ticks_ = 0;
	State* next_state_ = nullptr;
	std::unique_ptr<Surface> overlay_;
	unsigned fade_in_ticks_;
	unsigned fade_out_ticks_;
	unsigned fade_out_start_ticks_ = 0;
};

/// State that can be skipped with a button press
class SkipState : virtual public State
{
public:
	virtual void update(const Input& input) override;

protected:
	// Prevent skip for the first ticks
	unsigned grace_ticks_ = 15;
};

class SplashState : virtual public SkipState, virtual public State
{
public:
	// TODO: play sound when entering this state
	SplashState(std::vector<Surface*>& images, Window& window);
	
	virtual void draw(Window& window) const override;

private:
	std::vector<Surface*>& images_;
};

class TitleState : virtual public SkipState, virtual public State
{
	// TODO: make this state child of State, load menu on key press
public:
	TitleState(std::vector<Surface*>& images, Window& window);
	
	virtual void draw(Window& window) const override;
	
private:
	std::vector<Surface*>& images_;
	unsigned first_ticks_ = 50;
	unsigned scroll_ticks_ = 50;
	unsigned last_ticks_ = 50;
};

class GameState : public State
{
public:
	GameState(Game& game, SpriteManager& sprite_manager, Surface& game_surface, Window& window);

	virtual void update(const Input& input) override;
	virtual void draw(Window& window) const override;

private:
	Game& game_;
	Surface& game_surface_;
	GameRenderer game_renderer_;
	bool debug_info_ = false;
	bool paused_ = false;
	unsigned game_tick_ = 0;
};

// TODO: end state
