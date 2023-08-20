#include <gtest/gtest.h>

#include "sdl_stub.h"
#include "graphics.h"
#include "geometry.h"

using ::testing::_;
using ::testing::Return;
using ::testing::StrEq;

struct SDL_Window {};

class GraphicsTest : public ::testing::Test
{
 protected:
  void SetUp() override { SDLStub::get(); }
  void TearDown() override { SDLStub::reset(); }
};

TEST_F(GraphicsTest, window_create)
{
  SDL_Window sdl_window;
  const auto* window_name = "test";
  const auto window_width = 100;
  const auto window_height = 100;

  EXPECT_CALL(SDLStub::get(), SDL_CreateWindow(StrEq(window_name), _, _, window_width, window_height, _)).WillOnce(Return(&sdl_window));
  auto window = Window::create(window_name, geometry::Size(window_width, window_height));

  EXPECT_CALL(SDLStub::get(), SDL_DestroyWindow(&sdl_window));
  window.reset();
}

TEST_F(GraphicsTest, window_refresh)
{
  SDL_Window sdl_window;
  EXPECT_CALL(SDLStub::get(), SDL_CreateWindow(_, _, _, _, _, _)).WillOnce(Return(&sdl_window));
  auto window = Window::create("test", geometry::Size(100, 100));

  EXPECT_CALL(SDLStub::get(), SDL_UpdateWindowSurface(&sdl_window));
  window->refresh();

  EXPECT_CALL(SDLStub::get(), SDL_DestroyWindow(&sdl_window));
  window.reset();
}

TEST_F(GraphicsTest, surface_from_bmp)
{
  SDL_Surface sdl_surface;
  const auto* filename = "test";
  SDL_RWops sdl_rw_ops;
  EXPECT_CALL(SDLStub::get(), SDL_RWFromFile(StrEq(filename), _)).WillOnce(Return(&sdl_rw_ops));
  EXPECT_CALL(SDLStub::get(), SDL_LoadBMP_RW(&sdl_rw_ops, _)).WillOnce(Return(&sdl_surface));
  auto surface = Surface::from_bmp(filename);

  EXPECT_CALL(SDLStub::get(), SDL_FreeSurface(&sdl_surface));
  surface.reset();
}

TEST_F(GraphicsTest, surface_blit_surface)
{
  SDL_Window sdl_window;
  EXPECT_CALL(SDLStub::get(), SDL_CreateWindow(_, _, _, _, _, _)).WillOnce(Return(&sdl_window));
  auto window = Window::create("test", geometry::Size(100, 100));

  SDL_PixelFormat sdl_pixel_format;
  SDL_Surface sdl_window_surface;
  sdl_window_surface.format = &sdl_pixel_format;
  EXPECT_CALL(SDLStub::get(), SDL_GetWindowSurface(&sdl_window)).WillOnce(Return(&sdl_window_surface));
  auto window_surface = window->get_surface();

  SDL_Surface sdl_surface;
  EXPECT_CALL(SDLStub::get(), SDL_GetWindowSurface(&sdl_window)).WillOnce(Return(&sdl_window_surface));
  EXPECT_CALL(SDLStub::get(), SDL_CreateRGBSurface(_, _, _, _, _, _, _, _)).WillOnce(Return(&sdl_surface));
  auto surface = window->create_surface(geometry::Size(50, 50));

  // TODO: Check source / destination rectangles

  EXPECT_CALL(SDLStub::get(), SDL_BlitSurface(&sdl_surface, _, &sdl_window_surface, _));
  window_surface->blit_surface(surface.get(),
                               geometry::Rectangle(0, 0, 50, 50),
                               geometry::Rectangle(0, 0, 50, 50),
                               BlitType::CROP);

  EXPECT_CALL(SDLStub::get(), SDL_BlitScaled(&sdl_surface, _, &sdl_window_surface, _));
  window_surface->blit_surface(surface.get(),
                               geometry::Rectangle(0, 0, 50, 50),
                               geometry::Rectangle(0, 0, 100, 100),
                               BlitType::SCALE);

  EXPECT_CALL(SDLStub::get(), SDL_FreeSurface(&sdl_surface));
  surface.reset();

  EXPECT_CALL(SDLStub::get(), SDL_DestroyWindow(&sdl_window));
  window.reset();
}

TEST_F(GraphicsTest, surface_fill_rect)
{
  SDL_Window sdl_window;
  EXPECT_CALL(SDLStub::get(), SDL_CreateWindow(_, _, _, _, _, _)).WillOnce(Return(&sdl_window));
  auto window = Window::create("test", geometry::Size(100, 100));

  SDL_PixelFormat sdl_pixel_format;
  SDL_Surface sdl_window_surface;
  sdl_window_surface.format = &sdl_pixel_format;
  EXPECT_CALL(SDLStub::get(), SDL_GetWindowSurface(&sdl_window)).WillOnce(Return(&sdl_window_surface));
  auto window_surface = window->get_surface();

  EXPECT_CALL(SDLStub::get(), SDL_MapRGB(_, _, _, _));
  EXPECT_CALL(SDLStub::get(), SDL_FillRect(&sdl_window_surface, _, _));
  window_surface->fill_rect(geometry::Rectangle(0, 0, 100, 100), { 255u, 255u, 255u });

  EXPECT_CALL(SDLStub::get(), SDL_DestroyWindow(&sdl_window));
  window.reset();
}
