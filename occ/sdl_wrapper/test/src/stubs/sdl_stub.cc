#include "sdl_stub.h"

#include <functional>

#include <SDL.h>

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

SDL_Renderer* SDL_CreateRenderer(SDL_Window* window, int index, Uint32 flags)
{
  return SDLStub::get().SDL_CreateRenderer(window, index, flags);
}

void SDL_DestroyWindow(SDL_Window* window)
{
  SDLStub::get().SDL_DestroyWindow(window);
}

void SDL_DestroyRenderer(SDL_Renderer* renderer)
{
  SDLStub::get().SDL_DestroyRenderer(renderer);
}

void SDL_SetWindowSize(SDL_Window* window, int w, int h)
{
  SDLStub::get().SDL_SetWindowSize(window, w, h);
}

void SDL_RenderPresent(SDL_Renderer* renderer)
{
  SDLStub::get().SDL_RenderPresent(renderer);
}

int SDL_RenderDrawLine(SDL_Renderer* renderer, int x1, int y1, int x2, int y2)
{
  return SDLStub::get().SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
}

int SDL_RenderFillRect(SDL_Renderer* renderer, const SDL_Rect* rect)
{
  return SDLStub::get().SDL_RenderFillRect(renderer, rect);
}

int SDL_SetRenderDrawColor(SDL_Renderer* renderer, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
  return SDLStub::get().SDL_SetRenderDrawColor(renderer, r, g, b, a);
}

int SDL_SetRenderTarget(SDL_Renderer* renderer, SDL_Texture* texture)
{
  return SDLStub::get().SDL_SetRenderTarget(renderer, texture);
}

SDL_Surface* SDL_ConvertSurfaceFormat(SDL_Surface* src, Uint32 pixel_format, Uint32 flags)
{
  return SDLStub::get().SDL_ConvertSurfaceFormat(src, pixel_format, flags);
}

int SDL_LockSurface(SDL_Surface* surface)
{
  return SDLStub::get().SDL_LockSurface(surface);
}

void SDL_UnlockSurface(SDL_Surface* surface)
{
  SDLStub::get().SDL_UnlockSurface(surface);
}

void SDL_FreeSurface(SDL_Surface* surface)
{
  SDLStub::get().SDL_FreeSurface(surface);
}

void SDL_DestroyTexture(SDL_Texture* texture)
{
  SDLStub::get().SDL_DestroyTexture(texture);
}

SDL_Surface* SDL_CreateRGBSurface(Uint32 flags, int width, int height, int depth, Uint32 Rmask, Uint32 Gmask, Uint32 Bmask, Uint32 Amask)
{
  return SDLStub::get().SDL_CreateRGBSurface(flags, width, height, depth, Rmask, Gmask, Bmask, Amask);
}

SDL_Surface* SDL_CreateRGBSurfaceWithFormat(Uint32 flags, int width, int height, int depth, Uint32 format)
{
  return SDLStub::get().SDL_CreateRGBSurfaceWithFormat(flags, width, height, depth, format);
}

SDL_Surface* SDL_LoadBMP_RW(SDL_RWops* src, int freesrc)
{
  return SDLStub::get().SDL_LoadBMP_RW(src, freesrc);
}

SDL_Texture* SDL_CreateTexture(SDL_Renderer* renderer, Uint32 format, int access, int w, int h)
{
  return SDLStub::get().SDL_CreateTexture(renderer, format, access, w, h);
}

SDL_Texture* SDL_CreateTextureFromSurface(SDL_Renderer* renderer, SDL_Surface* surface)
{
  return SDLStub::get().SDL_CreateTextureFromSurface(renderer, surface);
}

SDL_RWops* SDL_RWFromFile(const char* file, const char* mode)
{
  return SDLStub::get().SDL_RWFromFile(file, mode);
}

int SDL_RWclose(SDL_RWops* context)
{
  return SDLStub::get().SDL_RWclose(context);
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

int SDL_LockTexture(SDL_Texture* texture, const SDL_Rect* rect, void** pixels, int* pitch)
{
  return SDLStub::get().SDL_LockTexture(texture, rect, pixels, pitch);
}

void SDL_UnlockTexture(SDL_Texture* texture)
{
  return SDLStub::get().SDL_UnlockTexture(texture);
}

int SDL_SetTextureBlendMode(SDL_Texture* texture, SDL_BlendMode blendMode)
{
  return SDLStub::get().SDL_SetTextureBlendMode(texture, blendMode);
}

int SDL_SetTextureColorMod(SDL_Texture* texture, Uint8 r, Uint8 g, Uint8 b)
{
  return SDLStub::get().SDL_SetTextureColorMod(texture, r, g, b);
}

int SDL_RenderCopy(SDL_Renderer* renderer, SDL_Texture* texture, const SDL_Rect* srcrect, const SDL_Rect* dstrect)
{
  return SDLStub::get().SDL_RenderCopy(renderer, texture, srcrect, dstrect);
}

int SDL_PollEvent(SDL_Event* event)
{
  return SDLStub::get().SDL_PollEvent(event);
}

const Uint8* SDL_GetKeyboardState(int* numkeys)
{
  return SDLStub::get().SDL_GetKeyboardState(numkeys);
}

Uint32 SDL_MapRGBA(const SDL_PixelFormat* format, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
  return SDLStub::get().SDL_MapRGBA(format, r, g, b, a);
}
