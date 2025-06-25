#include <core/Tile.h>
#include "config.h"

Tile createTile(SDL_Texture *texture, bool hasVariants, bool hasAnimations, int variantCount,
								int frameCount, float frameDuration) {
	int texW, texH;
	SDL_QueryTexture(texture, NULL, NULL, &texW, &texH);

	if (hasVariants && variantCount <= 0)
		variantCount = texW / TILE_SIZE;
	if (hasAnimations && frameCount <= 0)
		frameCount = texH / TILE_SIZE;

	Sprite sprite = createSprite(texture, hasVariants, hasAnimations, variantCount, frameCount, frameDuration);
	Tile tile = {.sprite = sprite};
	return tile;
}

void updateTile(Tile *tile, float deltaTime) {
	updateSprite(&tile->sprite, deltaTime);
}

void renderTile(SDL_Renderer *renderer, const Tile *tile, SDL_Rect dest) {
	renderSprite(renderer, &tile->sprite, dest);
}
