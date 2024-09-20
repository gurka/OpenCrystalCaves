#include "geometry.h"

namespace geometry
{

bool is_any_colliding(const std::vector<Rectangle>& v, const Rectangle& a)
{
  struct Collides
  {
    const Rectangle& r;
    constexpr Collides(const Rectangle& r) : r(r) {}
    constexpr bool operator()(const Rectangle& r2) const { return isColliding(r, r2); }
  };
  return std::any_of(v.cbegin(), v.cend(), Collides(a));
}

}
