#include "text.h"

#include <cassert>
#include <memory>

#include <SDL_ttf.h>

#include "logger.h"

namespace draw
{

void text(int x, int y, const std::string& text, const SDL_Color& color, SDL_Surface* dest)
{
  // Only open font once
  static std::unique_ptr<TTF_Font, decltype(&TTF_CloseFont)> font { nullptr, TTF_CloseFont };
  if (!font)
  {
    // TODO: Font size...
    font.reset(TTF_OpenFont("media/DejaVuSansMono.ttf", 12));
    if (!font)
    {
      LOG_CRITICAL("TTF_OpenFont failed with: '%s'", TTF_GetError());
      assert(font);
    }
  }

  // Render text to surface
  std::unique_ptr<SDL_Surface, decltype(&SDL_FreeSurface)> text_surface { TTF_RenderText_Solid(font.get(), text.c_str(), color), SDL_FreeSurface };

  // Render surface to destination
  SDL_Rect dest_rect = { x, y, 0, 0 };
  SDL_BlitSurface(text_surface.get(), nullptr, dest, &dest_rect);
}

}
