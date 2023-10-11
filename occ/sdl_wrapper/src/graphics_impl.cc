#include "graphics_impl.h"

#include <cassert>
#include <cstdlib>
#include <algorithm>
#include <utility>

#include "logger.h"
#include "occ_math.h"

SDL_Rect to_sdl_rect(const geometry::Rectangle& rect)
{
  return { rect.position.x(), rect.position.y(), rect.size.x(), rect.size.y() };
}

std::unique_ptr<Window> Window::create(const std::string& title, geometry::Size size, const std::string& text_sprite_filename)
{
  auto sdl_window = std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)>(SDL_CreateWindow(title.c_str(),
                                                                                               0,
                                                                                               0,
                                                                                               size.x(),
                                                                                               size.y(),
                                                                                               SDL_WINDOW_SHOWN),
                                                                              SDL_DestroyWindow);
  if (!sdl_window)
  {
	return nullptr;
  }
  auto sdl_renderer = std::unique_ptr<SDL_Renderer, decltype(&SDL_DestroyRenderer)>(SDL_CreateRenderer(sdl_window.get(),
																									   -1,
																									   0),
																					SDL_DestroyRenderer);
  if (!sdl_renderer)
  {
	return nullptr;
  }
  auto window = std::make_unique<WindowImpl>(std::move(sdl_window), std::move(sdl_renderer));
	if (!text_sprite_filename.empty())
	{
		window->set_text_sprite_filename(text_sprite_filename);
	}
  return window;
}

void WindowImpl::set_text_sprite_filename(const std::string& text_sprite_filename)
{
  text_surface_ = std::unique_ptr<Surface>(Surface::from_bmp(text_sprite_filename, *this));
	SDL_Color char_rgb = {0xFF, 0xFF, 0xFF, 0xFF};
  font_.BuildFromFile(sdl_renderer_.get(), text_sprite_filename.c_str(), char_rgb);
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
  auto sdl_texture = std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)>(SDL_CreateTexture(sdl_renderer_.get(),
																								   SDL_PIXELFORMAT_RGBA8888,
																								   SDL_TEXTUREACCESS_TARGET,
																								   size.x(),
																								   size.y()),
																				 SDL_DestroyTexture);
  if (!sdl_texture)
  {
	LOG_CRITICAL("Could not get texture: %s", SDL_GetError());
	return std::unique_ptr<Surface>();
  }
  return std::make_unique<SurfaceImpl>(size.x(), size.y(), std::move(sdl_texture), sdl_renderer_.get());
}

void WindowImpl::refresh()
{
  SDL_RenderPresent(sdl_renderer_.get());
}

void WindowImpl::fill_rect(const geometry::Rectangle& rect, const Color& color)
{
  const auto sdl_rect = to_sdl_rect(rect);
  // TODO: check error
  SDL_SetRenderDrawColor(sdl_renderer_.get(), color.red, color.green, color.blue, 0xff);
  SDL_RenderFillRect(sdl_renderer_.get(), &sdl_rect);
}

void WindowImpl::render_rectangle(const geometry::Rectangle& rect, const Color& color)
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

SDL_Color to_sdl_color(const Color& color)
{
  return { color.red, color.green, color.blue, 0xFF };
}

void WindowImpl::render_text(const geometry::Position& pos, const std::string& text, unsigned font_size, const Color& color)
{
  font_.RenderText(sdl_renderer_.get(), pos.x(), pos.y(), text.c_str(), font_size/16.0f, to_sdl_color(color));
}

void WindowImpl::render_line(const geometry::Position& from,
							 const geometry::Position& to,
							 const Color& color)
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
	LOG_CRITICAL("Could not load BMP: %s", SDL_GetError());
	return std::unique_ptr<Surface>();
  }
  auto sdl_renderer = static_cast<WindowImpl&>(window).get_renderer();
  auto sdl_texture = std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)>(SDL_CreateTextureFromSurface(sdl_renderer,
																											  sdl_surface.get()),
																				 SDL_DestroyTexture);
  if (!sdl_texture)
  {
	LOG_CRITICAL("Could not get texture: %s", SDL_GetError());
	return std::unique_ptr<Surface>();
  }
  return std::make_unique<SurfaceImpl>(sdl_surface->w, sdl_surface->h, std::move(sdl_texture), sdl_renderer);
}

std::unique_ptr<Surface> Surface::from_bmp(const std::string& filename, Window& window)
{
	return create_surface(SDL_LoadBMP(filename.c_str()), window);
}

std::unique_ptr<Surface> Surface::from_pixels(const int w, const int h, const uint32_t* pixels, Window& window)
{
	auto sdl_surface = SDL_CreateRGBSurfaceWithFormat(0, w, h, 32, SDL_PIXELFORMAT_ARGB8888);
	SDL_LockSurface(sdl_surface);
	memcpy(sdl_surface->pixels, pixels, w*h*sizeof(*pixels));
	SDL_UnlockSurface(sdl_surface);
	return create_surface(sdl_surface, window);
}

void SurfaceImpl::blit_surface(const geometry::Rectangle& source,
                               const geometry::Rectangle& dest) const
{
  const auto src_rect = to_sdl_rect(source);
  auto dest_rect = to_sdl_rect(dest);
  // TODO: check error
  SDL_RenderCopy(sdl_renderer_, sdl_texture_.get(), &src_rect, &dest_rect);
}

void SurfaceImpl::blit_surface() const
{
	// TODO: check error
	SDL_RenderCopy(sdl_renderer_, sdl_texture_.get(), nullptr, nullptr);
}

void SurfaceImpl::set_render_target()
{
  SDL_SetRenderTarget(sdl_renderer_, sdl_texture_.get());
}
