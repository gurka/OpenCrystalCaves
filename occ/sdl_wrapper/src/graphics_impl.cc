#include "graphics_impl.h"

#include <cassert>
#include <cstdlib>
#include <algorithm>
#include <utility>

#include "logger.h"
#include "occ_math.h"

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
  if (!sdl_surface)
  {
    LOG_CRITICAL("Could not load BMP: %s", SDL_GetError());
    return std::unique_ptr<Surface>();
  }
  else
  {
    return std::make_unique<SurfaceImpl>(std::move(sdl_surface));
  }
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

void SurfaceImpl::render_text(const geometry::Position& pos, const std::string& text, unsigned font_size, const Color& color)
{
  struct Font
  {
    unsigned size;
    std::unique_ptr<TTF_Font, decltype(&TTF_CloseFont)> font;
  };
  static std::vector<Font> fonts;

  // Check if requested font size already exist
  auto it = std::find_if(fonts.begin(), fonts.end(), [font_size](const Font& f)
  {
    return f.size == font_size;
  });

  if (it == fonts.end())
  {
    // Open requested font
    Font f { font_size, { nullptr, TTF_CloseFont } };
    f.font.reset(TTF_OpenFont("media/DejaVuSansMono.ttf", f.size));
    if (!f.font)
    {
      LOG_CRITICAL("TTF_OpenFont failed with: '%s'", TTF_GetError());
      assert(false);
    }

    // Insert and set iterator to point to it
    fonts.push_back(std::move(f));
    it = --fonts.end();
  }

  // Render text to surface
  std::unique_ptr<SDL_Surface, decltype(&SDL_FreeSurface)> text_surface
  {
    TTF_RenderText_Solid(it->font.get(), text.c_str(), to_sdl_color(color)),
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
  assert(from.x() == to.x() || from.y() == to.y());

  if (from.x() == to.x())
  {
    // Vertical line
    const SDL_Rect rect { from.x(), from.y(), 1, math::abs(from.y() - to.y()) };
    SDL_FillRect(sdl_surface_.get(),
                 &rect,
                 SDL_MapRGB(sdl_surface_->format, color.red, color.green, color.blue));
  }
  else if (from.y() == to.y())
  {
    // Horizontal line
    const SDL_Rect rect { from.x(), from.y(), math::abs(from.x() - to.x()), 1 };
    SDL_FillRect(sdl_surface_.get(),
                 &rect,
                 SDL_MapRGB(sdl_surface_->format, color.red, color.green, color.blue));
  }
}

void SurfaceImpl::render_rectangle(const geometry::Rectangle& rect, const Color& color)
{
  // top
  render_line(rect.position,
              geometry::Position(rect.position.x() + rect.size.x(),
                                 rect.position.y()),
              color);

  // bottom
  render_line(geometry::Position(rect.position.x(),
                                 rect.position.y() + rect.size.y() - 1),
              geometry::Position(rect.position.x() + rect.size.x(),
                                 rect.position.y() + rect.size.y() - 1),
              color);

  // left
  render_line(rect.position,
              geometry::Position(rect.position.x(),
                                 rect.position.y() + rect.size.y()),
              color);

  // right
  render_line(geometry::Position(rect.position.x() + rect.size.x() - 1,
                                 rect.position.y()),
              geometry::Position(rect.position.x() + rect.size.x() - 1,
                                 rect.position.y() + rect.size.y()),
              color);
}

SDL_Rect SurfaceImpl::to_sdl_rect(const geometry::Rectangle& rect) const
{
  return { rect.position.x(), rect.position.y(), rect.size.x(), rect.size.y() };
}

SDL_Color SurfaceImpl::to_sdl_color(const Color& color) const
{
  return { color.red, color.green, color.blue, 0u };
}
