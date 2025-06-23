#include <core/Sprite.h>
#include "config.h"
#include "stdlib.h"

Sprite createSprite(SDL_Texture *texture, bool hasVariants, bool hasAnimations, int variantCount, int frameCount,
                    int frameDuration) {
	Sprite sprite;
	sprite.texture = texture;

	sprite.hasVariants = hasVariants;
	sprite.variantCount = hasVariants ? variantCount : 0;
	sprite.variantIndex = hasVariants ? rand() % variantCount : 0;

	sprite.hasAnimations = hasAnimations;
	sprite.frameCount = hasAnimations ? frameCount : 0;
	sprite.frameDuration = hasAnimations ? frameDuration : 0;
	sprite.currentFrame = 0;
	sprite.lastFrameTime = SDL_GetTicks();

	return sprite;
}

void updateSprite(Sprite *sprite) {
	if (!sprite->hasAnimations) return;

	Uint32 now = SDL_GetTicks();
	if (now - sprite->lastFrameTime >= (Uint32) sprite->frameDuration) {
		sprite->currentFrame = (sprite->currentFrame + 1) % sprite->frameCount;
		sprite->lastFrameTime = now;
	}
}

void renderSprite(SDL_Renderer *renderer, const Sprite *sprite, SDL_Rect dest) {
	SDL_Rect src = {0, 0, TILE_SIZE, TILE_SIZE};

	if (sprite->hasVariants) {
		src.x = sprite->variantIndex * TILE_SIZE;
	}
	if (sprite->hasAnimations) {
		src.y = sprite->currentFrame * TILE_SIZE;
	}

	SDL_RenderCopy(renderer, sprite->texture, &src, &dest);
}
