#include <gtest/gtest.h>

#include "sdl_stub.h"
#include "graphics.h"
#include "geometry.h"

using ::testing::_;
using ::testing::Return;
using ::testing::StrEq;

struct SDL_Window {};
struct SDL_Renderer {};

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
  const auto text_sprite_filename = "foobar.bmp";

  EXPECT_CALL(SDLStub::get(), SDL_CreateWindow(StrEq(window_name), _, _, window_width, window_height, _)).WillOnce(Return(&sdl_window));
  SDL_Renderer sdl_renderer;
  EXPECT_CALL(SDLStub::get(), SDL_CreateRenderer(_, _, _)).WillOnce(Return(&sdl_renderer));
  auto window = Window::create(window_name, geometry::Size(window_width, window_height), text_sprite_filename);

  EXPECT_CALL(SDLStub::get(), SDL_DestroyWindow(&sdl_window));
  window.reset();
}

TEST_F(GraphicsTest, window_refresh)
{
  SDL_Window sdl_window;
  EXPECT_CALL(SDLStub::get(), SDL_CreateWindow(_, _, _, _, _, _)).WillOnce(Return(&sdl_window));
  SDL_Renderer sdl_renderer;
  EXPECT_CALL(SDLStub::get(), SDL_CreateRenderer(_, _, _)).WillOnce(Return(&sdl_renderer));
  auto window = Window::create("test", geometry::Size(100, 100), "foobar.bmp");

  EXPECT_CALL(SDLStub::get(), SDL_RenderPresent(&sdl_renderer));
  window->refresh();

  EXPECT_CALL(SDLStub::get(), SDL_DestroyWindow(&sdl_window));
  window.reset();
}

TEST_F(GraphicsTest, surface_from_bmp)
{
  SDL_Surface sdl_surface;
  const auto* filename = "test";
  SDL_RWops sdl_rw_ops;
  SDL_Window sdl_window;
  EXPECT_CALL(SDLStub::get(), SDL_CreateWindow(_, _, _, _, _, _)).WillOnce(Return(&sdl_window));
  SDL_Renderer sdl_renderer;
  EXPECT_CALL(SDLStub::get(), SDL_CreateRenderer(_, _, _)).WillOnce(Return(&sdl_renderer));
  auto window = Window::create("test", geometry::Size(100, 100), "foobar.bmp");
  EXPECT_CALL(SDLStub::get(), SDL_RWFromFile(StrEq(filename), _)).WillOnce(Return(&sdl_rw_ops));
  EXPECT_CALL(SDLStub::get(), SDL_LoadBMP_RW(&sdl_rw_ops, _)).WillOnce(Return(&sdl_surface));
  auto surface = Surface::from_bmp(filename, *window);

  surface.reset();
}

TEST_F(GraphicsTest, surface_fill_rect)
{
  SDL_Window sdl_window;
  EXPECT_CALL(SDLStub::get(), SDL_CreateWindow(_, _, _, _, _, _)).WillOnce(Return(&sdl_window));
  SDL_Renderer sdl_renderer;
  EXPECT_CALL(SDLStub::get(), SDL_CreateRenderer(_, _, _)).WillOnce(Return(&sdl_renderer));
  auto window = Window::create("test", geometry::Size(100, 100), "foobar.bmp");

  EXPECT_CALL(SDLStub::get(), SDL_SetRenderDrawColor(&sdl_renderer, _, _, _, _));
  EXPECT_CALL(SDLStub::get(), SDL_RenderFillRect(&sdl_renderer, _));
  window->fill_rect(geometry::Rectangle(0, 0, 100, 100), { 255u, 255u, 255u });

  EXPECT_CALL(SDLStub::get(), SDL_DestroyWindow(&sdl_window));
  EXPECT_CALL(SDLStub::get(), SDL_DestroyRenderer(&sdl_renderer));
  window.reset();
}
