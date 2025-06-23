#ifndef TILE_H
#define TILE_H

#include <SDL.h>
#include <Sprite.h>

typedef struct {
	Sprite sprite;
} Tile;

Tile createTile(SDL_Texture *texture, bool hasVariants, bool hasAnimations, int variantCount,
                int frameCount, int frameDuration);

void updateTile(Tile *tile);

void renderTile(SDL_Renderer *renderer, const Tile *tile, SDL_Rect dest);

#endif
