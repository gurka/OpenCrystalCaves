#ifndef DRAW_TEXT_H_
#define DRAW_TEXT_H_

#include <string>

class SDL_Color;
class SDL_Surface;

namespace draw
{

void text(int x, int y, const std::string& text, const SDL_Color& color, SDL_Surface* dest);

}

#endif  // DRAW_TEXT_H_
