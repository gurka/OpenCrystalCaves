#include <gmock/gmock.h>
#include <SDL.h>

class SDLStub
{
 public:
  static SDLStub& get();
  static void reset();

  MOCK_METHOD1(SDL_Init, int(Uint32));
  MOCK_METHOD0(SDL_GetError, const char*());
  MOCK_METHOD0(SDL_GetTicks, Uint32());
  MOCK_METHOD6(SDL_CreateWindow, SDL_Window*(const char*, int, int, int, int, Uint32));
  MOCK_METHOD3(SDL_CreateRenderer, SDL_Renderer*(SDL_Window*, int, Uint32));
  MOCK_METHOD1(SDL_DestroyWindow, void(SDL_Window*));
  MOCK_METHOD1(SDL_DestroyRenderer, void(SDL_Renderer*));
	MOCK_METHOD3(SDL_SetWindowSize, void(SDL_Window*, int, int));
  MOCK_METHOD1(SDL_RenderPresent, void(SDL_Renderer*));
  MOCK_METHOD5(SDL_RenderDrawLine, int(SDL_Renderer*, int, int, int, int));
  MOCK_METHOD2(SDL_RenderFillRect, int(SDL_Renderer*, const SDL_Rect*));
  MOCK_METHOD5(SDL_SetRenderDrawColor, int(SDL_Renderer*, Uint8, Uint8, Uint8, Uint8));
  MOCK_METHOD2(SDL_SetRenderTarget, int(SDL_Renderer*, SDL_Texture*));
  MOCK_METHOD3(SDL_ConvertSurfaceFormat, SDL_Surface*(SDL_Surface*, Uint32, Uint32));
	MOCK_METHOD1(SDL_LockSurface, int(SDL_Surface*));
	MOCK_METHOD1(SDL_UnlockSurface, void(SDL_Surface*));
  MOCK_METHOD1(SDL_FreeSurface, void(SDL_Surface*));
  MOCK_METHOD1(SDL_DestroyTexture, void(SDL_Texture*));
  MOCK_METHOD8(SDL_CreateRGBSurface, SDL_Surface*(Uint32, int, int, int, Uint32, Uint32, Uint32, Uint32));
	MOCK_METHOD5(SDL_CreateRGBSurfaceWithFormat, SDL_Surface*(Uint32, int, int, int, Uint32));
  MOCK_METHOD2(SDL_LoadBMP_RW, SDL_Surface*(SDL_RWops*, int));
  MOCK_METHOD5(SDL_CreateTexture, SDL_Texture*(SDL_Renderer*, Uint32, int, int, int));
  MOCK_METHOD2(SDL_CreateTextureFromSurface, SDL_Texture*(SDL_Renderer*, SDL_Surface*));
  MOCK_METHOD2(SDL_RWFromFile, SDL_RWops*(const char*, const char*));
  MOCK_METHOD1(SDL_RWclose, int(SDL_RWops*));
  MOCK_METHOD4(SDL_UpperBlit, int(SDL_Surface*, const SDL_Rect*, SDL_Surface*, SDL_Rect*));
  MOCK_METHOD4(SDL_UpperBlitScaled, int(SDL_Surface*, const SDL_Rect*, SDL_Surface*, SDL_Rect*));
  MOCK_METHOD4(SDL_MapRGB, Uint32(const SDL_PixelFormat*, Uint8, Uint8, Uint8));
  MOCK_METHOD3(SDL_FillRect, int(SDL_Surface*, const SDL_Rect*, Uint32));
  MOCK_METHOD4(SDL_LockTexture, int(SDL_Texture*, const SDL_Rect*, void**, int*));
  MOCK_METHOD1(SDL_UnlockTexture, void(SDL_Texture*));
  MOCK_METHOD2(SDL_SetTextureBlendMode, int(SDL_Texture*, SDL_BlendMode));
  MOCK_METHOD4(SDL_SetTextureColorMod, int(SDL_Texture*, Uint8, Uint8, Uint8));
  MOCK_METHOD4(SDL_RenderCopy, int(SDL_Renderer*, SDL_Texture*, const SDL_Rect*, const SDL_Rect*));

  MOCK_METHOD1(SDL_PollEvent, int(SDL_Event*));
  MOCK_METHOD5(SDL_MapRGBA, Uint32(const SDL_PixelFormat*, Uint8, Uint8, Uint8, Uint8));
  MOCK_METHOD1(SDL_GetKeyboardState, const Uint8*(int*));

 private:
  SDLStub() = default;

  static std::unique_ptr<SDLStub> stub;
};
