#ifndef MENU_H_
#define MENU_H_

#include <array>

#include <SDL.h>

#include "input.h"

class Menu
{
 public:
  enum class Result
  {
    NO_RESULT,
    NEW_GAME,
    HELP,
    QUIT,
  };

  Menu();

  Result update(const Input& input);
  void render(SDL_Surface* screen);

 private:
  static const int number_menu_items_ = 3;
  static const std::array<std::string, number_menu_items_> menu_items_;

  std::array<SDL_Surface*, number_menu_items_> menu_item_white_surfaces_;
  std::array<SDL_Surface*, number_menu_items_> menu_item_red_surfaces_;
  bool changed_;
  int selected_menu_item_;
};

#endif  // MENU_H_
