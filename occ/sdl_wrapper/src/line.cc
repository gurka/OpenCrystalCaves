#include "line.h"

#include <SDL.h>

#include <cassert>

#include "geometry.h"
#include "math.h"

namespace draw
{

void line(int from_x, int from_y, int to_x, int to_y, const SDL_Color& color, SDL_Surface* dest)
{
  assert(from_x == to_x || from_y == to_y);

  if (from_x == to_x)
  {
    // Vertical line
    const SDL_Rect rect { from_x, from_y, 1, math::abs(from_y - to_y) };
    SDL_FillRect(dest, &rect, SDL_MapRGB(dest->format, color.r, color.g, color.b));
  }
  else if (from_y == to_y)
  {
    // Horizontal line
    const SDL_Rect rect { from_x, from_y, math::abs(from_x - to_x), 1 };
    SDL_FillRect(dest, &rect, SDL_MapRGB(dest->format, color.r, color.g, color.b));
  }
}

void rectangle(const geometry::Rectangle& rectangle, const SDL_Color& color, SDL_Surface* dest)
{
  assert(rectangle.size.x() > 0 && rectangle.size.y() > 0);

  // top
  line(rectangle.position.x(),
       rectangle.position.y(),
       rectangle.position.x() + rectangle.size.x(),
       rectangle.position.y(),
       color,
       dest);

  // bottom
  line(rectangle.position.x(),
       rectangle.position.y() + rectangle.size.y() - 1,
       rectangle.position.x() + rectangle.size.x(),
       rectangle.position.y() + rectangle.size.y() - 1,
       color,
       dest);

  // left
  line(rectangle.position.x(),
       rectangle.position.y(),
       rectangle.position.x(),
       rectangle.position.y() + rectangle.size.y(),
       color,
       dest);

  // right
  line(rectangle.position.x() + rectangle.size.x() - 1,
       rectangle.position.y(),
       rectangle.position.x() + rectangle.size.x() - 1,
       rectangle.position.y() + rectangle.size.y(),
       color,
       dest);
}

}
