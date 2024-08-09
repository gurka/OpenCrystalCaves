#pragma once
#include <utility>

#include "geometry.h"
#include "misc.h"
#include "sprite.h"

class Enemy
{
 public:
  Enemy(geometry::Position position, int health, int points) : position(std::move(position)), health(health), points(points) {}
  virtual ~Enemy() = default;

  virtual void update() = 0;
  virtual Sprite get_sprite() const = 0;

  geometry::Position position;
  int health;
  int points;
};

class Hopper : public Enemy
{
  // Moves left and right erratically
 public:
  Hopper(geometry::Position position) : Enemy(position, 1, 100) {}

  virtual void update() override;
  virtual Sprite get_sprite() const override;

 private:
  bool left_ = false;
  int frame_ = 0;
};

class Snake : public Enemy
{
  // Moves left/right, pauses, leaves slime
 public:
  Snake(geometry::Position position) : Enemy(position, 2, 100) {}

  virtual void update() override;
  virtual Sprite get_sprite() const override;

 private:
  bool left_ = false;
  bool paused_ = false;
  int frame_ = 0;
};

class Spider : public Enemy
{
  // Moves up and down, shoots webs ahead
 public:
  Spider(geometry::Position position) : Enemy(position, 1, 100) {}

  virtual void update() override;
  virtual Sprite get_sprite() const override;

 private:
  bool up_ = false;
  int frame_ = 0;
};
