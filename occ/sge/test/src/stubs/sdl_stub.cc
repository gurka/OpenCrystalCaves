#include "sdl_stub.h"

#include <functional>

#include <SDL.h>
#include <SDL_ttf.h>

std::unique_ptr<SDLStub> SDLStub::stub;

SDLStub& SDLStub::get()
{
  if (!stub)
  {
    // make_unique does not work with private constructor...
    stub.reset(new SDLStub());
  }
  return *stub;
}

void SDLStub::reset()
{
  stub.reset();
}

int SDL_Init(Uint32 flags)
{
  return SDLStub::get().SDL_Init(flags);
}

void SDL_Quit()
{
  // Do not mock this since SDL_Quit is called only in atexit
}

const char* SDL_GetError()
{
  return SDLStub::get().SDL_GetError();
}

Uint32 SDL_GetTicks()
{
  return SDLStub::get().SDL_GetTicks();
}

SDL_Window* SDL_CreateWindow(const char* title, int x, int y, int w, int h, Uint32 flags)
{
  return SDLStub::get().SDL_CreateWindow(title, x, y, w, h, flags);
}

void SDL_DestroyWindow(SDL_Window* window)
{
  SDLStub::get().SDL_DestroyWindow(window);
}

SDL_Surface* SDL_GetWindowSurface(SDL_Window* window)
{
  return SDLStub::get().SDL_GetWindowSurface(window);
}

int SDL_UpdateWindowSurface(SDL_Window* window)
{
  return SDLStub::get().SDL_UpdateWindowSurface(window);
}

void SDL_FreeSurface(SDL_Surface* surface)
{
  SDLStub::get().SDL_FreeSurface(surface);
}

SDL_Surface* SDL_CreateRGBSurface(Uint32 flags, int width, int height, int depth,
                                  Uint32 Rmask, Uint32 Gmask, Uint32 Bmask, Uint32 Amask)
{
  return SDLStub::get().SDL_CreateRGBSurface(flags, width, height, depth, Rmask, Gmask, Bmask, Amask);
}

SDL_Surface* SDL_LoadBMP_RW(SDL_RWops* src, int freesrc)
{
  return SDLStub::get().SDL_LoadBMP_RW(src, freesrc);
}

SDL_RWops* SDL_RWFromFile(const char* file, const char* mode)
{
  return SDLStub::get().SDL_RWFromFile(file, mode);
}

int SDL_UpperBlit(SDL_Surface* src, const SDL_Rect* srcrect, SDL_Surface* dst, SDL_Rect* dstrect)
{
  return SDLStub::get().SDL_UpperBlit(src, srcrect, dst, dstrect);
}

int SDL_UpperBlitScaled(SDL_Surface* src, const SDL_Rect* srcrect, SDL_Surface* dst, SDL_Rect* dstrect)
{
  return SDLStub::get().SDL_UpperBlitScaled(src, srcrect, dst, dstrect);
}

Uint32 SDL_MapRGB(const SDL_PixelFormat* format, Uint8 r, Uint8 g, Uint8 b)
{
  return SDLStub::get().SDL_MapRGB(format, r, g, b);
}

int SDL_FillRect(SDL_Surface* dst, const SDL_Rect* rect, Uint32 color)
{
  return SDLStub::get().SDL_FillRect(dst, rect, color);
}

int SDL_PollEvent(SDL_Event* event)
{
  return SDLStub::get().SDL_PollEvent(event);
}

int TTF_Init()
{
  return SDLStub::get().TTF_Init();
}

TTF_Font* TTF_OpenFont(const char* file, int ptsize)
{
  return SDLStub::get().TTF_OpenFont(file, ptsize);
}

void TTF_CloseFont(TTF_Font* font)
{
  // Do not mock this since TTF_CloseFont is called in static destructor (on exit)
}

SDL_Surface* TTF_RenderText_Solid(TTF_Font* font, const char* text, SDL_Color fg)
{
  return SDLStub::get().TTF_RenderText_Solid(font, text, fg);
}
