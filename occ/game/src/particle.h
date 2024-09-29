#pragma once

#include "geometry.h"
#include "misc.h"
#include "sprite.h"

class Particle
{
 public:
  Particle(geometry::Position position) : position(position) {}
  virtual ~Particle() = default;

  virtual void update() = 0;
  virtual int get_sprite() const = 0;
  virtual bool is_alive() const = 0;

  geometry::Position position;
};

class Explosion : public Particle
{
 public:
  Explosion(geometry::Position position) : Particle(position) {}

  virtual void update() override;
  virtual int get_sprite() const override;
  virtual bool is_alive() const override;

 private:
  unsigned frame_ = 0;
  static constexpr auto sprites_ = misc::make_array(28, 29, 30, 31, 30, 29, 28);
};

class ScoreParticle : public Particle
{
 public:
  ScoreParticle(geometry::Position position, int score);

  virtual void update() override;
  virtual int get_sprite() const override;
  virtual bool is_alive() const override;

 private:
  unsigned frame_ = 0;
  Sprite sprite_;
};
