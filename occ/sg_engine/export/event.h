#ifndef EVENT_H_
#define EVENT_H_

#include <memory>

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
    bool pressed  = false;
    bool repeated = false;
  };

  Button up    = Button();
  Button down  = Button();
  Button left  = Button();
  Button right = Button();
  Button z     = Button();
  Button x     = Button();

  Button num_1 = Button();
  Button num_2 = Button();

  Button enter = Button();
  Button space = Button();

  bool quit    = false;
};

#endif  // EVENT_H_
