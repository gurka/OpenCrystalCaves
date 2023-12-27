#pragma once

#include "sdl_wrapper.h"
#include "event.h"
#include "graphics.h"
#include "spritemgr.h"
#include "game_renderer.h"

#include "game.h"

/// Represents a game state (e.g. splash, title, game)
class State
{
 public:
	State() {}
	virtual ~State() = 0;

	void set_next(State& next) { next_state_ = &next; }

	// Called whenever this state is entered
	virtual void reset() { ticks_ = 0; }

	// Update and return next state
	virtual State* update(const Input& input);
	
	// Called whenever state exits
	virtual void finish() {}

	virtual void draw(Window& window) const = 0;

protected:
	unsigned ticks_ = 0;
	State* next_state_ = nullptr;
};

/// State that can be skipped with a button press
class SkipState : public State
{
public:
	virtual State* update(const Input& input) override;

protected:
	// Prevent skip for the first ticks
	unsigned grace_ticks_ = 30;
};

class SplashState : public SkipState
{
public:
	// TODO: play sound when entering this state
	SplashState(std::vector<Surface*>& images, Window& window);
	
	virtual void draw(Window& window) const override;

private:
	std::vector<Surface*>& images_;
	std::unique_ptr<Surface> overlay_;
};

class TitleState : public SkipState
{
	// TODO: make this state child of State, load menu on key press
public:
	TitleState(std::vector<Surface*>& images) : images_(images) {}
	
	virtual void draw(Window& window) const override;
	
private:
	std::vector<Surface*>& images_;
	unsigned first_ticks_ = 100;
	unsigned scroll_ticks_ = 30;
	unsigned last_ticks_ = 100;
};

class GameState : public State
{
public:
	GameState(Game& game, SpriteManager& sprite_manager, Surface& game_surface, Window& window);

	virtual State* update(const Input& input) override;
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
