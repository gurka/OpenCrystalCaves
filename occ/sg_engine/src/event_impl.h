#ifndef EVENT_IMPL_H_
#define EVENT_IMPL_H_

#include "event.h"

class EventImpl : public Event
{
 public:
  void poll_event(Input* input) override;
};

#endif  // EVENT_IMPL_H_
