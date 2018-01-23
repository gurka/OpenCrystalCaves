#ifndef GEOMETRY_H_
#define GEOMETRY_H_

#include <utility>

#include "vector.h"

namespace geometry
{

// Represents a position in 2d space
using Position = Vector<int>;

// Represents an objects size (x is width, y is height)
using Size = Vector<int>;

struct Rectangle
{
  constexpr Rectangle()
    : position(),
      size()
  {
  }

  constexpr Rectangle(Position position, Size size)
    : position(std::move(position)),
      size(std::move(size))
  {
  }

  constexpr Rectangle(Position position, int width, int height)
    : position(std::move(position)),
      size(width, height)
  {
  }

  constexpr Rectangle(int x, int y, Size size)
    : position(x, y),
      size(std::move(size))
  {
  }

  constexpr Rectangle(int x, int y, int width, int height)
    : position(x, y),
      size(width, height)
  {
  }

  Position position;
  Size size;
};

// Returns true if Rectangle a and Rectangle b intersect
constexpr bool isColliding(const Rectangle& a, const Rectangle& b)
{
  return a.position.x() < b.position.x() + b.size.x() &&
         a.position.y() < b.position.y() + b.size.y() &&
         a.position.x() + a.size.x() > b.position.x() &&
         a.position.y() + a.size.y() > b.position.y();
}

}

#endif  // GEOMETRY_H_
