#ifndef TILEMAP_H
#define TILEMAP_H

#include <world/TileMapLayer.h>
#include <world/TileLayer.h>

typedef struct {
	int width;
	int height;
	int layerCount;
	TileMapLayer layers[LAYER_COUNT];
} TileMap;

TileMap createTileMap(int width, int height);

void destroyTileMap(TileMap *map);

void renderTileMap(SDL_Renderer *renderer, TileMap *map);

void updateTileMap(TileMap *map);

#endif
