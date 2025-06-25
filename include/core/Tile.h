#ifndef TILE_H
#define TILE_H

#include <SDL.h>
#include <Sprite.h>

typedef struct
{
  Sprite sprite;
} Tile;

Tile createTile(SDL_Texture* texture, bool hasVariants, bool hasAnimations, int variantCount,
                int frameCount, float frameDuration); // <- float for seconds

void updateTile(Tile* tile, float deltaTime); // <- now takes deltaTime

void renderTile(SDL_Renderer* renderer, const Tile* tile, SDL_Rect dest);

#endif
