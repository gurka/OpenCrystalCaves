#pragma once

#include <memory>

#include "geometry.h"

struct Input;

class Event
{
 public:
  static std::unique_ptr<Event> create();

  virtual ~Event() = default;

  virtual void poll_event(Input* input) = 0;
};

struct Input
{
  struct Button
  {
    bool down = false;
    bool repeated = false;

    bool pressed() const { return down && !repeated; }
    void tick() { repeated = down; }
    void set_down(const bool d)
    {
      down = d;
      if (!d)
        repeated = false;
    }
  };

  Button up = Button();
  Button down = Button();
  Button left = Button();
  Button right = Button();
  Button z = Button();
  Button x = Button();

  Button num_1 = Button();
  Button num_2 = Button();

  Button enter = Button();
  Button space = Button();
  Button escape = Button();

  // Cheat code buttons
  Button noclip = Button();
  Button ammo = Button();
  Button godmode = Button();
  Button reverse_gravity = Button();

  geometry::Position mouse;
};
