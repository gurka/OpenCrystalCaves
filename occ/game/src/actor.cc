#include "actor.h"

#include "level.h"

std::vector<geometry::Rectangle> Actor::create_detection_rects(const int dx, const int dy, const Level& level) const
{
  // Create rectangles originating from this actor extending toward a cardinal direction,
  // until there is a solid collision.
  std::vector<geometry::Rectangle> rects;
  if (dx == 1)
  {
    // right
    int y = position.y();
    for (int remain = size.y(); remain > 0; remain -= 16)
    {
      const int h = std::min(remain, 16);
      geometry::Rectangle r{position.x() + 16, y, 0, h};
      y += h;
      for (;;)
      {
        auto r_new = r;
        r_new.size += geometry::Size{16, 0};
        // TODO: check camera for inside
        if (level.collides_solid(r_new.position, r_new.size) ||
            !geometry::is_inside(r_new, geometry::Rectangle(0, 0, level.width * 16, level.height * 16)))
        {
          rects.push_back(r);
          break;
        }
        r = r_new;
      }
    }
  }
  else if (dx == -1)
  {
    // left
    int y = position.y();
    for (int remain = size.y(); remain > 0; remain -= 16)
    {
      const int h = std::min(remain, 16);
      geometry::Rectangle r{position.x(), y, 0, h};
      y += h;
      for (;;)
      {
        auto r_new = r;
        r_new.position -= geometry::Position{16, 0};
        r_new.size += geometry::Size{16, 0};
        if (level.collides_solid(r_new.position, r_new.size) ||
            !geometry::is_inside(r_new, geometry::Rectangle(0, 0, level.width * 16, level.height * 16)))
        {
          rects.push_back(r);
          break;
        }
        r = r_new;
      }
    }
  }
  else if (dy == 1)
  {
    // down
    int x = position.x();
    for (int remain = size.x(); remain > 0; remain -= 16)
    {
      const int w = std::min(remain, 16);
      geometry::Rectangle r{x, position.y() + 16, w, 0};
      x += w;
      for (;;)
      {
        auto r_new = r;
        r_new.size += geometry::Size{0, 16};
        if (level.collides_solid(r_new.position, r_new.size) ||
            !geometry::is_inside(r_new, geometry::Rectangle(0, 0, level.width * 16, level.height * 16)))
        {
          rects.push_back(r);
          break;
        }
        r = r_new;
      }
    }
  }
  else if (dy == -1)
  {
    // up
    int x = position.x();
    for (int remain = size.x(); remain > 0; remain -= 16)
    {
      const int w = std::min(remain, 16);
      geometry::Rectangle r{x, position.y(), w, 0};
      x += w;
      for (;;)
      {
        auto r_new = r;
        r_new.position -= geometry::Position{0, 16};
        r_new.size += geometry::Size{0, 16};
        if (level.collides_solid(r_new.position, r_new.size) ||
            !geometry::is_inside(r_new, geometry::Rectangle(0, 0, level.width * 16, level.height * 16)))
        {
          rects.push_back(r);
          break;
        }
        r = r_new;
      }
    }
  }
  return rects;
}