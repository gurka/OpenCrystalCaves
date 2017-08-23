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

// Returns true if Rectangle a is inside Rectangle b ("strict" inside, e.g. can't occupy same position)
constexpr bool isStrictInside(const Rectangle& a, const Rectangle& b)
{
  return a.position.getX() > b.position.getX() &&
         a.position.getY() > b.position.getY() &&
         a.position.getX() + a.size.getX() < b.position.getX() + b.size.getX() &&
         a.position.getY() + a.size.getY() < b.position.getY() + b.size.getY();
}

constexpr bool isInside(const Rectangle& a, const Rectangle& b)
{
  return a.position.getX() >= b.position.getX() &&
         a.position.getY() >= b.position.getY() &&
         a.position.getX() + a.size.getX() <= b.position.getX() + b.size.getX() &&
         a.position.getY() + a.size.getY() <= b.position.getY() + b.size.getY();
}

// Returns true if Rectangle a and Rectangle b intersect
constexpr bool isColliding(const Rectangle& a, const Rectangle& b)
{
  return a.position.getX() < b.position.getX() + b.size.getX() &&
         a.position.getY() < b.position.getY() + b.size.getY() &&
         a.position.getX() + a.size.getX() > b.position.getX() &&
         a.position.getY() + a.size.getY() > b.position.getY();
}

}

#endif  // GEOMETRY_H_
