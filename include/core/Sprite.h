#ifndef SPRITE_H
#define SPRITE_H

#include <SDL.h>

typedef struct
{
  SDL_Texture* texture;

  bool hasVariants;
  bool hasAnimations;

  int variantCount;
  int variantIndex;

  int frameCount; // total frames
  float frameDuration; // duration of one frame in seconds
  int currentFrame;
  float frameTimer; // accumulates time between frames
} Sprite;

Sprite createSprite(SDL_Texture* texture, bool hasVariants, bool hasAnimations,
                    int variantCount, int frameCount, float frameDuration);

void updateSprite(Sprite* sprite, float deltaTime);

void renderSprite(SDL_Renderer* renderer, const Sprite* sprite, SDL_Rect dest);

#endif
