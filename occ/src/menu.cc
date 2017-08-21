#include "menu.h"

#include <SDL_ttf.h>

#include "config.h"

decltype(Menu::menu_items_) Menu::menu_items_ =
{
  "New Game",
  "Help",
  "Quit"
};

Menu::Menu()
  : changed_(false),
    selected_menu_item_(0u)
{
  // Create TTF_Font and a SDL_Color
  // Might need to change 32 if we change base screen resolution
  auto* font = TTF_OpenFont("DejaVuSans.ttf", 28);
  assert(font != nullptr);
  const SDL_Color color_white = { 255, 255, 255 };
  const SDL_Color color_red = { 196, 8, 8 };

  // Create SDL_Surface for each menu item
  for (auto i = 0u; i < number_menu_items_; i++)
  {
    menu_item_white_surfaces_[i] = TTF_RenderText_Solid(font, menu_items_[i].c_str(), color_white);
    assert(menu_item_white_surfaces_[i] != nullptr);
    menu_item_red_surfaces_[i] = TTF_RenderText_Solid(font, menu_items_[i].c_str(), color_red);
    assert(menu_item_red_surfaces_[i] != nullptr);
  }
}

Menu::Result Menu::update(const Input& input)
{
//  // Reset changed_ and return on no input
//  if (!input.down.pressed && !input.up.pressed && !input.space)
//  {
//    changed_ = false;
//    return Result::NO_RESULT;
//  }
//
//  // Don't accept more input if user hasn't released a button
//  if (changed_ == true)
//  {
//    return Result::NO_RESULT;
//  }
//
//  if (input.down)
//  {
//    selected_menu_item_ += 1;
//    if (selected_menu_item_ >= number_menu_items_)
//    {
//      selected_menu_item_ = 0;
//    }
//    changed_ = true;
//  }
//  else if (input.up)
//  {
//    selected_menu_item_ -= 1;
//    if (selected_menu_item_ < 0)
//    {
//      selected_menu_item_ = number_menu_items_ - 1;
//    }
//    changed_ = true;
//  }
//  else if (input.space)
//  {
//    switch (selected_menu_item_)
//    {
//      case 0: return Result::NEW_GAME;
//      case 1: return Result::HELP;
//      case 2: return Result::QUIT;
//    }
//    assert(selected_menu_item_ >= 0 && selected_menu_item_ <= 2);
//  }

  return Result::NO_RESULT;
}

void Menu::render(SDL_Surface* screen)
{
  // Figure out where to render the menu items
  const auto x = 32;
  auto y = (config::CAMERA_HEIGHT * config::SCREEN_SCALE) - 32 - (number_menu_items_ * 32);

  // Render each menu item
  for (auto i = 0; i < number_menu_items_; i++)
  {
    SDL_Rect dest_rect = { x, y, 0, 0 };

    if (selected_menu_item_ == i)
    {
      SDL_BlitSurface(menu_item_red_surfaces_[i], nullptr, screen, &dest_rect);
    }
    else
    {
      SDL_BlitSurface(menu_item_white_surfaces_[i], nullptr, screen, &dest_rect);
    }

    y += 32;
  }
}
