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
	input->up.tick();
	input->down.tick();
	input->left.tick();
	input->right.tick();
	input->z.tick();
	input->x.tick();
	input->num_1.tick();
	input->num_2.tick();
	input->enter.tick();
	input->space.tick();
	input->escape.tick();
	input->noclip.tick();
	input->ammo.tick();
	input->godmode.tick();
	input->reverse_gravity.tick();

  // Read 
  const auto keys = SDL_GetKeyboardState(nullptr);
  SDL_Event event;
  while (SDL_PollEvent(&event) != 0)
  {
    if (event.type == SDL_QUIT)
    {
		input->escape.set_down(true);
    }

    else if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP)
    {
      switch (event.key.keysym.sym)
      {
        case SDLK_UP:
			  input->up.set_down(event.type == SDL_KEYDOWN);
          break;

        case SDLK_DOWN:
			  input->down.set_down(event.type == SDL_KEYDOWN);
          break;

        case SDLK_LEFT:
			  input->left.set_down(event.type == SDL_KEYDOWN);
          break;

        case SDLK_RIGHT:
			  input->right.set_down(event.type == SDL_KEYDOWN);
          break;

        case SDLK_z:
			  input->z.set_down(event.type == SDL_KEYDOWN);
          break;

        case SDLK_x:
			  input->x.set_down(event.type == SDL_KEYDOWN);
          break;

        case SDLK_g:
			  input->reverse_gravity.set_down(event.type == SDL_KEYDOWN);
	      break;

        case SDLK_1:
			  input->num_1.set_down(event.type == SDL_KEYDOWN);
          break;

        case SDLK_2:
			  input->num_2.set_down(event.type == SDL_KEYDOWN);
          break;

        case SDLK_RETURN:
			  input->enter.set_down(event.type == SDL_KEYDOWN);
          break;

        case SDLK_SPACE:
			  input->space.set_down(event.type == SDL_KEYDOWN);
          break;

        case SDLK_ESCAPE:
			  input->escape.set_down(event.type == SDL_KEYDOWN);
			  break;

        default:
          break;
      }
    }
  }
  input->noclip.down = keys[SDL_SCANCODE_I] && keys[SDL_SCANCODE_L] && keys[SDL_SCANCODE_M];
  input->ammo.down = keys[SDL_SCANCODE_X] && keys[SDL_SCANCODE_T] && keys[SDL_SCANCODE_R] && keys[SDL_SCANCODE_A];
  input->godmode.down = keys[SDL_SCANCODE_Z] && keys[SDL_SCANCODE_E] && keys[SDL_SCANCODE_U] && keys[SDL_SCANCODE_S];
}
