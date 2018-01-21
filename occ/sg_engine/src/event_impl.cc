#include "event_impl.h"

#include <memory>
#include <utility>

#include "SDL.h"

std::unique_ptr<Event> Event::create()
{
  return std::make_unique<EventImpl>();
}

void EventImpl::poll_event(Input* input)
{
  // Set any input that is pressed as repeated here
  input->up.repeated    = input->up.pressed;
  input->down.repeated  = input->down.pressed;
  input->left.repeated  = input->left.pressed;
  input->right.repeated = input->right.pressed;
  input->z.repeated     = input->z.pressed;
  input->x.repeated     = input->x.pressed;
  input->num_1.repeated = input->num_1.pressed;
  input->num_2.repeated = input->num_2.pressed;
  input->enter.repeated = input->enter.pressed;
  input->space.repeated = input->space.pressed;

  // Read events
  SDL_Event event;
  while (SDL_PollEvent(&event) != 0)
  {
    if (event.type == SDL_QUIT)
    {
      input->quit = true;
      return;  // Quit asap
    }

    else if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP)
    {
      switch (event.key.keysym.sym)
      {
        case SDLK_UP:
          input->up.pressed = event.type == SDL_KEYDOWN;
          if (!input->up.pressed) input->up.repeated = false;
          break;

        case SDLK_DOWN:
          input->down.pressed = event.type == SDL_KEYDOWN;
          if (!input->down.pressed) input->down.repeated = false;
          break;

        case SDLK_LEFT:
          input->left.pressed = event.type == SDL_KEYDOWN;
          if (!input->left.pressed) input->left.repeated = false;
          break;

        case SDLK_RIGHT:
          input->right.pressed = event.type == SDL_KEYDOWN;
          if (!input->right.pressed) input->right.repeated = false;
          break;

        case SDLK_z:
          input->z.pressed = event.type == SDL_KEYDOWN;
          if (!input->z.pressed) input->z.repeated = false;
          break;

        case SDLK_x:
          input->x.pressed = event.type == SDL_KEYDOWN;
          if (!input->x.pressed) input->x.repeated = false;
          break;

        case SDLK_1:
          input->num_1.pressed = event.type == SDL_KEYDOWN;
          if (!input->num_1.pressed) input->num_1.repeated = false;
          break;

        case SDLK_2:
          input->num_2.pressed = event.type == SDL_KEYDOWN;
          if (!input->num_2.pressed) input->num_2.repeated = false;
          break;

        case SDLK_RETURN:
          input->enter.pressed = event.type == SDL_KEYDOWN;
          if (!input->enter.pressed) input->enter.repeated = false;
          break;

        case SDLK_SPACE:
          input->space.pressed = event.type == SDL_KEYDOWN;
          if (!input->space.pressed) input->space.repeated = false;
          break;

        case SDLK_ESCAPE:
          input->quit = true;
          return;  // Quit asap

        default:
          break;
      }
    }
  }
}
