#ifndef TILEMAPLAYER_H
#define TILEMAPLAYER_H

#include <core/Tile.h>

typedef struct {
	int width;
	int height;
	Tile **tiles;
} TileMapLayer;

TileMapLayer createTileMapLayer(int width, int height);

void destroyTileMapLayer(TileMapLayer *layer);

void renderTileMapLayer(SDL_Renderer *renderer, TileMapLayer *layer);

#endif
