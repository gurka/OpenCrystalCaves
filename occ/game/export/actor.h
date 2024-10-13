// Base class of enemies and hazards
#pragma once
#include <utility>

#include "geometry.h"
#include "misc.h"
#include "sprite.h"

enum class LeverColor : int
{
  LEVER_COLOR_R = 0,
  LEVER_COLOR_B = 1,
  LEVER_COLOR_G = 2,
};

struct Level;

class Actor
{
 public:
  Actor(geometry::Position position, geometry::Size size) : position(std::move(position)), size(std::move(size)) {}
  virtual ~Actor() = default;

  virtual bool is_alive() const { return true; }
  virtual int get_points() const { return 0; }

  virtual void update(const geometry::Rectangle& player_rect, Level& level) = 0;
  virtual bool interact([[maybe_unused]] Level& level) { return false; };
  virtual std::vector<std::pair<geometry::Position, Sprite>> get_sprites(const Level& level) const = 0;
  virtual std::vector<geometry::Rectangle> get_detection_rects([[maybe_unused]] const Level& level) const { return {}; }

  geometry::Position position;
  geometry::Size size;

 protected:
  std::vector<geometry::Rectangle> create_detection_rects(const int dx, const int dy, const Level& level) const;
};

class Lever : public Actor
{
  // ⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛🟥🚨🟥⬛⬛⬛
  // ⬛⬛⬛⬛⬛⬛⬛⬛⬛🟥🚨🟥🟥🟥⬛⬛
  // ⬛⬛⬛⬛⬛⬛⬛⬛⬛🟥🚨🟥🟥🟥⬛⬛
  // ⬛⬛⬛⬛⬛⬛⬛⬛⬛🟥🟥🟥🟥🟥⬛⬛
  // ⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛🟥🟥🟥⬛⬛⬛
  // ⬛⬛⬛⬛⬛⬛⬛⬛⬛⬛⚪🟥⬛⬛⬛⬛
  // ⬛⬛⬛⬛⬛⬛⬛⬛⬛⚪🪦⬛⬛⬛⬛⬛
  // ⬛⬛⬛⬛⬛⬛⬛⬛⬛⚪🪦⬛⬛⬛⬛⬛
  // ⬛⬛⬛⬛⬛⬛⬛⬛⚪🪦⬛⬛⬛⬛⬛⬛
  // ⬛⬛⬛⬛⬛⬛⬛⬛⚪🪦⬛⬛⬛⬛⬛⬛
  // ⬛⬛⬛⬛⬛⬛⬛⬛⚪🪦⬛⬛⬛⬛⬛⬛
  // ⬛⬛⬛⬛⬛⬛⬛⚪🪦⬛⬛⬛⬛⬛⬛⬛
  // ⬛⬛⬛⬛⬛⬛⬛⚪🪦⬛⬛⬛⬛⬛⬛⬛
  // ⬛⬛⬛⬛⬛⬛⬛⚪🪦⬛⬛⬛⬛⬛⬛⬛
  // ⬛⬛⬛🪦🪦⚪⚪⬜⬜⚪⚪🪦🪦⬛⬛⬛
  // Opens a door when interacted with
 public:
  Lever(geometry::Position position, LeverColor color) : Actor(position, geometry::Size(16, 16)), color_(color) {}

  virtual bool interact(Level& level) override;
  virtual std::vector<std::pair<geometry::Position, Sprite>> get_sprites(const Level& level) const override;
  virtual void update([[maybe_unused]] const geometry::Rectangle& player_rect, [[maybe_unused]] Level& level) override {}

 private:
  LeverColor color_;
};
