#ifndef SPRITE_H
#define SPRITE_H

#include <SDL.h>

typedef struct {
	SDL_Texture *texture;

	bool hasVariants;
	bool hasAnimations;

	int variantCount;
	int variantIndex;

	int frameCount;
	int frameDuration;
	int currentFrame;
	Uint32 lastFrameTime;
} Sprite;

Sprite createSprite(SDL_Texture *texture, bool hasVariants, bool hasAnimations, int variantCount, int frameCount,
                    int frameDuration);

void updateSprite(Sprite *sprite);

void renderSprite(SDL_Renderer *renderer, const Sprite *sprite, SDL_Rect dest);

#endif
