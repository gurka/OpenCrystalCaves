#include <gmock/gmock.h>
#include <SDL.h>
#include <SDL_ttf.h>

class SDLStub
{
 public:
  static SDLStub& get();
  static void reset();

  MOCK_METHOD1(SDL_Init, int(Uint32));
  MOCK_METHOD0(SDL_GetError, const char*());
  MOCK_METHOD0(SDL_GetTicks, Uint32());
  MOCK_METHOD6(SDL_CreateWindow, SDL_Window*(const char*, int, int, int, int, Uint32));
  MOCK_METHOD1(SDL_DestroyWindow, void(SDL_Window*));
  MOCK_METHOD1(SDL_GetWindowSurface, SDL_Surface*(SDL_Window*));
  MOCK_METHOD1(SDL_UpdateWindowSurface, int(SDL_Window*));
  MOCK_METHOD1(SDL_FreeSurface, void(SDL_Surface*));
  MOCK_METHOD8(SDL_CreateRGBSurface, SDL_Surface*(Uint32, int, int, int, Uint32, Uint32, Uint32, Uint32));
  MOCK_METHOD2(SDL_LoadBMP_RW, SDL_Surface*(SDL_RWops*, int));
  MOCK_METHOD2(SDL_RWFromFile, SDL_RWops*(const char*, const char*));
  MOCK_METHOD4(SDL_UpperBlit, int(SDL_Surface*, const SDL_Rect*, SDL_Surface*, SDL_Rect*));
  MOCK_METHOD4(SDL_UpperBlitScaled, int(SDL_Surface*, const SDL_Rect*, SDL_Surface*, SDL_Rect*));
  MOCK_METHOD4(SDL_MapRGB, Uint32(const SDL_PixelFormat*, Uint8, Uint8, Uint8));
  MOCK_METHOD3(SDL_FillRect, int(SDL_Surface*, const SDL_Rect*, Uint32));

  MOCK_METHOD1(SDL_PollEvent, int(SDL_Event*));

  MOCK_METHOD0(TTF_Init, int());
  MOCK_METHOD2(TTF_OpenFont, TTF_Font*(const char*, int));
  MOCK_METHOD1(TTF_CloseFont, void(TTF_Font*));
  MOCK_METHOD3(TTF_RenderText_Solid, SDL_Surface*(TTF_Font*, const char*, SDL_Color));

 private:
  SDLStub() = default;

  static std::unique_ptr<SDLStub> stub;
};
