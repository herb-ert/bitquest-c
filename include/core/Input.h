#ifndef INPUT_H
#define INPUT_H

#include <SDL.h>

typedef struct
{
  bool up;
  bool down;
  bool left;
  bool right;
  bool interact;
  bool pause;
} InputState;

void resetInput(InputState* input);

void handleKeyDown(InputState* input, SDL_Keycode key);

void handleKeyUp(InputState* input, SDL_Keycode key);

#endif
