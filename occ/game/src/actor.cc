#include "actor.h"

#include "level.h"

geometry::Rectangle Actor::create_detection_rect(const int dx, const int dy, const Level& level) const
{
  // Create a rectangle originating from this actor extending toward a cardinal direction,
  // until there is a solid collision.
  if (dx == 1)
  {
    // right
    geometry::Rectangle r{position.x() + 16, position.y(), 0, size.y()};
    for (;;)
    {
      auto r_new = r;
      r_new.size += geometry::Size{16, 0};
      // TODO: check camera for inside
      if (level.collides_solid(r_new.position, r_new.size) ||
          !geometry::is_inside(r_new, geometry::Rectangle(0, 0, level.width * 16, level.height * 16)))
      {
        return r;
      }
      r = r_new;
    }
  }
  else if (dx == -1)
  {
    // left
    geometry::Rectangle r{position.x(), position.y(), 0, size.y()};
    for (;;)
    {
      auto r_new = r;
      r_new.position -= geometry::Position{16, 0};
      r_new.size += geometry::Size{16, 0};
      if (level.collides_solid(r_new.position, r_new.size) ||
          !geometry::is_inside(r_new, geometry::Rectangle(0, 0, level.width * 16, level.height * 16)))
      {
        return r;
      }
      r = r_new;
    }
  }
  else if (dy == 1)
  {
    // down
    geometry::Rectangle r{position.x(), position.y() + 16, size.x(), 0};
    for (;;)
    {
      auto r_new = r;
      r_new.size += geometry::Size{0, 16};
      if (level.collides_solid(r_new.position, r_new.size) ||
          !geometry::is_inside(r_new, geometry::Rectangle(0, 0, level.width * 16, level.height * 16)))
      {
        return r;
      }
      r = r_new;
    }
  }
  else if (dy == -1)
  {
    // up
    geometry::Rectangle r{position.x(), position.y(), size.x(), 0};
    for (;;)
    {
      auto r_new = r;
      r_new.position -= geometry::Position{0, 16};
      r_new.size += geometry::Size{0, 16};
      if (level.collides_solid(r_new.position, r_new.size) ||
          !geometry::is_inside(r_new, geometry::Rectangle(0, 0, level.width * 16, level.height * 16)))
      {
        return r;
      }
      r = r_new;
    }
  }
  return geometry::Rectangle();
}