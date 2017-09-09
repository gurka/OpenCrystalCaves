#ifndef DRAW_LINE_H_
#define DRAW_LINE_H_

class SDL_Color;
class SDL_Surface;
namespace geometry
{
class Rectangle;
}

namespace draw
{

void line(int from_x, int from_y, int to_x, int to_y, const SDL_Color& color, SDL_Surface* dest);
void rectangle(const geometry::Rectangle& rectangle, const SDL_Color& color, SDL_Surface* dest);

}

#endif  // DRAW_LINE_H_
