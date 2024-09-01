#include "graphics_impl.h"

#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <utility>

#include "logger.h"
#include "misc.h"
#include "occ_math.h"

SDL_Rect to_sdl_rect(const geometry::Rectangle& rect)
{
  return {rect.position.x(), rect.position.y(), rect.size.x(), rect.size.y()};
}

std::unique_ptr<Window> Window::create(const std::string& title, geometry::Size size, const std::filesystem::path& icon_path)
{
  auto sdl_window = std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)>(
    SDL_CreateWindow(title.c_str(), 0, 0, size.x(), size.y(), SDL_WINDOW_SHOWN), SDL_DestroyWindow);
  if (!sdl_window)
  {
    return nullptr;
  }
  auto icon = IMG_Load(icon_path.string().c_str());
  if (!icon)
  {
    LOG_ERROR("could not load icon file %s", icon_path.string().c_str());
  }
  else
  {
    LOG_DEBUG("setting window icon file %s", icon_path.string().c_str());
    SDL_SetWindowIcon(sdl_window.get(), icon);
    SDL_FreeSurface(icon);
  }
  auto sdl_renderer =
    std::unique_ptr<SDL_Renderer, decltype(&SDL_DestroyRenderer)>(SDL_CreateRenderer(sdl_window.get(), -1, 0), SDL_DestroyRenderer);
  if (!sdl_renderer)
  {
    return nullptr;
  }
  auto window = std::make_unique<WindowImpl>(std::move(sdl_window), std::move(sdl_renderer));
  return window;
}

void WindowImpl::set_size(geometry::Size size)
{
  // TODO: check error
  SDL_SetWindowSize(sdl_window_.get(), size.x(), size.y());
}

void WindowImpl::set_render_target(Surface* surface)
{
  if (surface)
  {
    static_cast<SurfaceImpl*>(surface)->set_render_target();
  }
  else
  {
    SDL_SetRenderTarget(sdl_renderer_.get(), nullptr);
  }
}

std::unique_ptr<Surface> WindowImpl::create_target_surface(geometry::Size size)
{
  auto sdl_texture = std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)>(
    SDL_CreateTexture(sdl_renderer_.get(), SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, size.x(), size.y()), SDL_DestroyTexture);
  if (!sdl_texture)
  {
    LOG_CRITICAL("Could not get texture: %s", SDL_GetError());
    return std::unique_ptr<Surface>();
  }
  return std::make_unique<SurfaceImpl>(size.x(), size.y(), std::move(sdl_texture), *sdl_renderer_.get());
}

void WindowImpl::refresh()
{
  SDL_RenderPresent(sdl_renderer_.get());
}

void WindowImpl::fill_rect(const geometry::Rectangle& rect, const Color& color)
{
  const auto sdl_rect = to_sdl_rect(rect);
  // TODO: check error
  SDL_SetRenderDrawColor(sdl_renderer_.get(), color.red, color.green, color.blue, color.alpha);
  SDL_RenderFillRect(sdl_renderer_.get(), &sdl_rect);
}

void WindowImpl::render_rectangle(const geometry::Rectangle& rect, const Color& color)
{
  // top
  render_line(rect.position, geometry::Position(rect.position.x() + rect.size.x() - 1, rect.position.y()), color);

  // bottom
  render_line(geometry::Position(rect.position.x(), rect.position.y() + rect.size.y() - 1),
              geometry::Position(rect.position.x() + rect.size.x() - 1, rect.position.y() + rect.size.y() - 1),
              color);

  // left
  render_line(rect.position, geometry::Position(rect.position.x(), rect.position.y() + rect.size.y() - 1), color);

  // right
  render_line(geometry::Position(rect.position.x() + rect.size.x() - 1, rect.position.y()),
              geometry::Position(rect.position.x() + rect.size.x() - 1, rect.position.y() + rect.size.y() - 1),
              color);
}

SDL_Color to_sdl_color(const Color& color)
{
  return {color.red, color.green, color.blue, 0xFF};
}

void WindowImpl::render_line(const geometry::Position& from, const geometry::Position& to, const Color& color)
{
  // TODO: check error
  SDL_SetRenderDrawColor(sdl_renderer_.get(), color.red, color.green, color.blue, 0xff);
  SDL_RenderDrawLine(sdl_renderer_.get(), from.x(), from.y(), to.x(), to.y());
}

std::unique_ptr<Surface> create_surface(SDL_Surface* surface, Window& window)
{
  auto sdl_surface = std::unique_ptr<SDL_Surface, decltype(&SDL_FreeSurface)>(surface, SDL_FreeSurface);
  if (!sdl_surface)
  {
    LOG_CRITICAL("Could not load surface: %s", SDL_GetError());
    return std::unique_ptr<Surface>();
  }
  auto sdl_renderer = static_cast<WindowImpl&>(window).get_renderer();
  auto sdl_texture = std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)>(
    SDL_CreateTextureFromSurface(sdl_renderer, sdl_surface.get()), SDL_DestroyTexture);
  if (!sdl_texture)
  {
    LOG_CRITICAL("Could not get texture: %s", SDL_GetError());
    return std::unique_ptr<Surface>();
  }
  return std::make_unique<SurfaceImpl>(sdl_surface->w, sdl_surface->h, std::move(sdl_texture), *sdl_renderer);
}

std::unique_ptr<Surface> Surface::from_bmp(const std::filesystem::path& filename, Window& window)
{
  return create_surface(SDL_LoadBMP(filename.string().c_str()), window);
}

std::unique_ptr<Surface> Surface::from_image(const std::filesystem::path& filename, Window& window)
{
  auto sdl_surface = IMG_Load(filename.string().c_str());
  return create_surface(sdl_surface, window);
}

std::unique_ptr<Surface> Surface::from_pixels(const int w, const int h, const uint32_t* pixels, Window& window)
{
  auto sdl_surface = SDL_CreateRGBSurfaceWithFormat(0, w, h, 32, SDL_PIXELFORMAT_ARGB8888);
  if (pixels)
  {
    // TODO: check error
    SDL_LockSurface(sdl_surface);
    memcpy(sdl_surface->pixels, pixels, w * h * sizeof(*pixels));
    SDL_UnlockSurface(sdl_surface);
  }
  return create_surface(sdl_surface, window);
}

SurfaceImpl::SurfaceImpl(const int w,
                         const int h,
                         std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)> sdl_texture,
                         SDL_Renderer& sdl_renderer)
  : w_(w),
    h_(h),
    sdl_texture_(std::move(sdl_texture)),
    sdl_renderer_(sdl_renderer)
{
  if (SDL_SetTextureBlendMode(sdl_texture_.get(), SDL_BLENDMODE_BLEND) != 0)
  {
    LOG_ERROR("Could not set texture blend mode: %s", SDL_GetError());
  }
}

void SurfaceImpl::blit_surface(const geometry::Rectangle& source, const geometry::Rectangle& dest, const bool flip, const Color tint) const
{
	if (SDL_SetTextureColorMod(sdl_texture_.get(), tint.red, tint.green, tint.blue) != 0)
	{
		LOG_ERROR("Could not set texture color mod: %s", SDL_GetError());
	}
  const auto src_rect = to_sdl_rect(source);
  auto dest_rect = to_sdl_rect(dest);
  const SDL_RendererFlip sdl_flip = flip ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
  if (SDL_RenderCopyEx(&sdl_renderer_, sdl_texture_.get(), &src_rect, &dest_rect, 0.0, nullptr, sdl_flip) != 0)
  {
	  LOG_ERROR("Could not render texture: %s", SDL_GetError());
  }
	// reset tint
	if (SDL_SetTextureColorMod(sdl_texture_.get(), 0xff, 0xff, 0xff) != 0)
	{
		LOG_ERROR("Could not reset texture color mod: %s", SDL_GetError());
	}
}

void SurfaceImpl::blit_surface() const
{
  // TODO: check error
  SDL_RenderCopy(&sdl_renderer_, sdl_texture_.get(), nullptr, nullptr);
}

void SurfaceImpl::set_render_target()
{
  SDL_SetRenderTarget(&sdl_renderer_, sdl_texture_.get());
}

void SurfaceImpl::set_alpha(const uint8_t alpha)
{
  SDL_SetTextureAlphaMod(sdl_texture_.get(), alpha);
}
