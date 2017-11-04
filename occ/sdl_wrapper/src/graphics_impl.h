#ifndef GRAPHICS_IMPL_H_
#define GRAPHICS_IMPL_H_

#include "graphics.h"

#include <memory>
#include <utility>

#include <SDL.h>
#include <SDL_ttf.h>

#include "geometry.h"

class WindowImpl : public Window
{
 public:
  WindowImpl(std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)> sdl_window)
    : sdl_window_(std::move(sdl_window))
  {
  }

  std::unique_ptr<Surface> get_surface() override;
  void refresh() override;

  std::unique_ptr<Surface> create_surface(geometry::Size size) override;

 private:
  std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)> sdl_window_;
};

class SurfaceImpl : public Surface
{
 public:
  SurfaceImpl(std::unique_ptr<SDL_Surface, decltype(&SDL_FreeSurface)> sdl_surface)
    : sdl_surface_(std::move(sdl_surface))
  {
  }

  int width() const override { return sdl_surface_->w; }
  int height() const override { return sdl_surface_->h; }

  void blit_surface(const Surface* surface,
                    const geometry::Rectangle& source,
                    const geometry::Rectangle& dest,
                    BlitType blit_type) override;
  void fill_rect(const geometry::Rectangle& rect, const Color& color) override;
  void render_text(const geometry::Position& pos, const std::string& text, unsigned font_size, const Color& color) override;
  void render_line(const geometry::Position& from, const geometry::Position& to, const Color& color) override;
  void render_rectangle(const geometry::Rectangle& rect, const Color& color) override;

 private:
  SDL_Rect to_sdl_rect(const geometry::Rectangle& rect) const;
  SDL_Color to_sdl_color(const Color& color) const;

  std::unique_ptr<SDL_Surface, decltype(&SDL_FreeSurface)> sdl_surface_;
};

#endif  // GRAPHICS_IMPL_H_
