#include "graphics_impl.h"

#include <cassert>
#include <cstdlib>
#include <utility>

#include "logger.h"

std::unique_ptr<Window> Window::create(const std::string& title, geometry::Size size)
{
  auto sdl_window = std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)>(SDL_CreateWindow(title.c_str(),
                                                                                               0,
                                                                                               0,
                                                                                               size.x(),
                                                                                               size.y(),
                                                                                               SDL_WINDOW_SHOWN),
                                                                              SDL_DestroyWindow);
  return std::make_unique<WindowImpl>(std::move(sdl_window));
}

std::unique_ptr<Surface> WindowImpl::get_surface()
{
  auto sdl_surface = std::unique_ptr<SDL_Surface, decltype(&SDL_FreeSurface)>(SDL_GetWindowSurface(sdl_window_.get()),
                                                                              [](SDL_Surface*){});
  return std::make_unique<SurfaceImpl>(std::move(sdl_surface));
}

void WindowImpl::refresh()
{
  SDL_UpdateWindowSurface(sdl_window_.get());
}

std::unique_ptr<Surface> WindowImpl::create_surface(geometry::Size size)
{
  auto* window_surface = SDL_GetWindowSurface(sdl_window_.get());
  auto sdl_surface = std::unique_ptr<SDL_Surface, decltype(&SDL_FreeSurface)>(SDL_CreateRGBSurface(0,
                                                                                                   size.x(),
                                                                                                   size.y(),
                                                                                                   window_surface->format->BitsPerPixel,
                                                                                                   window_surface->format->Rmask,
                                                                                                   window_surface->format->Gmask,
                                                                                                   window_surface->format->Bmask,
                                                                                                   window_surface->format->Amask),
                                                                              SDL_FreeSurface);
  return std::make_unique<SurfaceImpl>(std::move(sdl_surface));
}

std::unique_ptr<Surface> Surface::from_bmp(const std::string& filename)
{
  auto sdl_surface = std::unique_ptr<SDL_Surface, decltype(&SDL_FreeSurface)>(SDL_LoadBMP(filename.c_str()),
                                                                              SDL_FreeSurface);
  return std::make_unique<SurfaceImpl>(std::move(sdl_surface));
}

void SurfaceImpl::blit_surface(const Surface* surface,
                               const geometry::Rectangle& source,
                               const geometry::Rectangle& dest,
                               BlitType blit_type)
{
  auto* surface_impl = static_cast<const SurfaceImpl*>(surface);
  const auto src_rect = to_sdl_rect(source);
  auto dest_rect = to_sdl_rect(dest);
  switch (blit_type)
  {
    case BlitType::CROP:
      SDL_BlitSurface(surface_impl->sdl_surface_.get(), &src_rect, sdl_surface_.get(), &dest_rect);
      break;
    case BlitType::SCALE:
      SDL_BlitScaled(surface_impl->sdl_surface_.get(), &src_rect, sdl_surface_.get(), &dest_rect);
      break;
  }
}

void SurfaceImpl::fill_rect(const geometry::Rectangle& rect, const Color& color)
{
  const auto sdl_rect = to_sdl_rect(rect);
  SDL_FillRect(sdl_surface_.get(), &sdl_rect, SDL_MapRGB(sdl_surface_->format, color.red, color.green, color.blue));
}

void SurfaceImpl::render_text(const geometry::Position& pos, const std::string& text, const Color& color)
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
  std::unique_ptr<SDL_Surface, decltype(&SDL_FreeSurface)> text_surface
  {
    TTF_RenderText_Solid(font.get(), text.c_str(), to_sdl_color(color)),
    SDL_FreeSurface
  };

  // Render surface to destination
  SDL_Rect dest_rect = { pos.x(), pos.y(), 0, 0 };
  SDL_BlitSurface(text_surface.get(), nullptr, sdl_surface_.get(), &dest_rect);
}

void SurfaceImpl::render_line(const geometry::Position& from,
                              const geometry::Position& to,
                              const Color& color)
{
  (void)from;
  (void)to;
  (void)color;
  // TODO
//  assert(from_x == to_x || from_y == to_y);
//
//  if (from_x == to_x)
//  {
//    // Vertical line
//    const SDL_Rect rect { from_x, from_y, 1, math::abs(from_y - to_y) };
//    SDL_FillRect(dest, &rect, SDL_MapRGB(dest->format, color.r, color.g, color.b));
//  }
//  else if (from_y == to_y)
//  {
//    // Horizontal line
//    const SDL_Rect rect { from_x, from_y, math::abs(from_x - to_x), 1 };
//    SDL_FillRect(dest, &rect, SDL_MapRGB(dest->format, color.r, color.g, color.b));
//  }
}

void SurfaceImpl::render_rectangle(const geometry::Rectangle& rect, const Color& color)
{
  (void)rect;
  (void)color;
  // TODO
//  assert(rectangle.size.x() > 0 && rectangle.size.y() > 0);
//
//  // top
//  render_line(rectangle.position.x(),
//       rectangle.position.y(),
//       rectangle.position.x() + rectangle.size.x(),
//       rectangle.position.y(),
//       color);
//
//  // bottom
//  render_line(rectangle.position.x(),
//       rectangle.position.y() + rectangle.size.y() - 1,
//       rectangle.position.x() + rectangle.size.x(),
//       rectangle.position.y() + rectangle.size.y() - 1,
//       color;
//
//  // left
//  render_line(rectangle.position.x(),
//       rectangle.position.y(),
//       rectangle.position.x(),
//       rectangle.position.y() + rectangle.size.y(),
//       color);
//
//  // right
//  render_line(rectangle.position.x() + rectangle.size.x() - 1,
//       rectangle.position.y(),
//       rectangle.position.x() + rectangle.size.x() - 1,
//       rectangle.position.y() + rectangle.size.y(),
//       color);
}

SDL_Rect SurfaceImpl::to_sdl_rect(const geometry::Rectangle& rect) const
{
  return { rect.position.x(), rect.position.y(), rect.size.x(), rect.size.y() };
}

SDL_Color SurfaceImpl::to_sdl_color(const Color& color) const
{
  return { color.red, color.green, color.blue, 0u };
}
