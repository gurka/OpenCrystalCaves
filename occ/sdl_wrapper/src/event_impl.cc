#include "event_impl.h"

#include <memory>
#include <utility>

#include <SDL.h>

std::unique_ptr<Event> Event::create()
{
  return std::make_unique<EventImpl>();
}

void EventImpl::poll_event(Input* input)
{
  // Set any input that is pressed as repeated here
  input->up.repeated = input->up.down;
  input->down.repeated = input->down.down;
  input->left.repeated = input->left.down;
  input->right.repeated = input->right.down;
  input->z.repeated = input->z.down;
  input->x.repeated = input->x.down;
  input->num_1.repeated = input->num_1.down;
  input->num_2.repeated = input->num_2.down;
  input->enter.repeated = input->enter.down;
  input->space.repeated = input->space.down;
  input->noclip.repeated = input->noclip.down;
  input->ammo.repeated = input->ammo.down;
  input->godmode.repeated = input->godmode.down;
  input->reverse_gravity.repeated = input->reverse_gravity.down;

  // Read 
  const auto keys = SDL_GetKeyboardState(nullptr);
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
          input->up.down = event.type == SDL_KEYDOWN;
          if (!input->up.down)
            input->up.repeated = false;
          break;

        case SDLK_DOWN:
          input->down.down = event.type == SDL_KEYDOWN;
          if (!input->down.down)
            input->down.repeated = false;
          break;

        case SDLK_LEFT:
          input->left.down = event.type == SDL_KEYDOWN;
          if (!input->left.down)
            input->left.repeated = false;
          break;

        case SDLK_RIGHT:
          input->right.down = event.type == SDL_KEYDOWN;
          if (!input->right.down)
            input->right.repeated = false;
          break;

        case SDLK_z:
          input->z.down = event.type == SDL_KEYDOWN;
          if (!input->z.down)
            input->z.repeated = false;
          break;

        case SDLK_x:
          input->x.down = event.type == SDL_KEYDOWN;
          if (!input->x.down)
            input->x.repeated = false;
          break;

        case SDLK_g:
          input->reverse_gravity.down = event.type == SDL_KEYDOWN;
	      break;

        case SDLK_1:
          input->num_1.down = event.type == SDL_KEYDOWN;
          if (!input->num_1.down)
            input->num_1.repeated = false;
          break;

        case SDLK_2:
          input->num_2.down = event.type == SDL_KEYDOWN;
          if (!input->num_2.down)
            input->num_2.repeated = false;
          break;

        case SDLK_RETURN:
          input->enter.down = event.type == SDL_KEYDOWN;
          if (!input->enter.down)
            input->enter.repeated = false;
          break;

        case SDLK_SPACE:
          input->space.down = event.type == SDL_KEYDOWN;
          if (!input->space.down)
            input->space.repeated = false;
          break;

        case SDLK_ESCAPE:
          input->quit = true;
          return;  // Quit asap

        default:
          break;
      }
    }
  }
  input->noclip.down = keys[SDL_SCANCODE_I] && keys[SDL_SCANCODE_L] && keys[SDL_SCANCODE_M];
  input->ammo.down = keys[SDL_SCANCODE_X] && keys[SDL_SCANCODE_T] && keys[SDL_SCANCODE_R] && keys[SDL_SCANCODE_A];
  input->godmode.down = keys[SDL_SCANCODE_Z] && keys[SDL_SCANCODE_E] && keys[SDL_SCANCODE_U] && keys[SDL_SCANCODE_S];
}
