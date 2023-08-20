#ifndef GRAPHICS_H_
#define GRAPHICS_H_

#include <string>
#include <memory>

#include "geometry.h"

class Surface;

struct Color
{
  std::uint8_t red   = 0u;
  std::uint8_t green = 0u;
  std::uint8_t blue  = 0u;
};

class Window
{
 public:
  static std::unique_ptr<Window> create(const std::string& title, geometry::Size size, const std::string& text_sprite_filename);

  virtual ~Window() = default;

  virtual void refresh() = 0;
  virtual void fill_rect(const geometry::Rectangle& rect, const Color& color) = 0;
  virtual void render_text(const geometry::Position& pos, const std::string& text, unsigned font_size, const Color& color) = 0;
  virtual void render_line(const geometry::Position& from, const geometry::Position& to, const Color& color) = 0;
  virtual void render_rectangle(const geometry::Rectangle& rect, const Color& color) = 0;
};

enum class BlitType
{
  CROP,
  SCALE
};

class Surface
{
 public:
  static std::unique_ptr<Surface> from_bmp(const std::string& filename, Window& window);

  virtual ~Surface() = default;

  virtual int width() const = 0;
  virtual int height() const = 0;

  virtual void blit_surface(const geometry::Rectangle& source,
                            const geometry::Rectangle& dest) const = 0;
};

#endif  // GRAPHICS_H_
