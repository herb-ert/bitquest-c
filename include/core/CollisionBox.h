#ifndef COLLISIONBOX_H
#define COLLISIONBOX_H

#include <SDL.h>

typedef struct
{
  float x, y, w, h;
} CollisionBox;

void renderCollisionBox(SDL_Renderer* renderer, float x, float y, CollisionBox box);

#endif
