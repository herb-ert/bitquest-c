#ifndef TILE_H
#define TILE_H

#include <SDL.h>
#include <core/Sprite.h>
#include <core/CollisionBox.h>

typedef struct
{
  Sprite sprite;
  CollisionBox collisionBox;
} Tile;

Tile createTile(SDL_Texture* texture, bool hasVariants, bool hasAnimations,
                int variantCount, int frameCount, float frameDuration, CollisionBox box);

void updateTile(Tile* tile, float deltaTime);
void renderTile(SDL_Renderer* renderer, const Tile* tile, SDL_Rect dest);

#endif
