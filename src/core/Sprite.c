#include <core/Sprite.h>
#include "config.h"
#include "stdlib.h"

Sprite createSprite(SDL_Texture *texture, bool hasVariants, bool hasAnimations,
										int variantCount, int frameCount, float frameDuration) {
	Sprite sprite;
	sprite.texture = texture;

	sprite.hasVariants = hasVariants;
	sprite.variantCount = hasVariants ? variantCount : 0;
	sprite.variantIndex = hasVariants ? rand() % variantCount : 0;

	sprite.hasAnimations = hasAnimations;
	sprite.frameCount = hasAnimations ? frameCount : 0;
	sprite.frameDuration = hasAnimations ? frameDuration : 0.0f;
	sprite.currentFrame = 0;
	sprite.frameTimer = 0.0f;

	return sprite;
}

void updateSprite(Sprite *sprite, float deltaTime) {
	if (!sprite->hasAnimations || sprite->frameCount <= 1) return;

	sprite->frameTimer += deltaTime;

	while (sprite->frameTimer >= sprite->frameDuration) {
		sprite->frameTimer -= sprite->frameDuration;
		sprite->currentFrame = (sprite->currentFrame + 1) % sprite->frameCount;
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
