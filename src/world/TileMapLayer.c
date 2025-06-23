#include <world/TileMapLayer.h>
#include <config.h>
#include <stdlib.h>

TileMapLayer createTileMapLayer(int width, int height) {
	TileMapLayer layer;
	layer.width = width;
	layer.height = height;

	layer.tiles = (Tile **) malloc(sizeof(Tile *) * height);
	for (int y = 0; y < height; y++) {
		layer.tiles[y] = (Tile *) calloc(width, sizeof(Tile));
	}

	return layer;
}

void destroyTileMapLayer(TileMapLayer *layer) {
	for (int y = 0; y < layer->height; y++) {
		free(layer->tiles[y]);
	}
	free(layer->tiles);
	layer->tiles = NULL;
}

void renderTileMapLayer(SDL_Renderer *renderer, TileMapLayer *layer) {
	SDL_Rect dest = {0, 0, (int) (TILE_SIZE * SCALE), (int) (TILE_SIZE * SCALE)};

	for (int y = 0; y < layer->height; y++) {
		for (int x = 0; x < layer->width; x++) {
			Tile *tile = &layer->tiles[y][x];
			if (tile->sprite.texture != NULL) {
				dest.x = (int) (x * TILE_SIZE * SCALE);
				dest.y = (int) (y * TILE_SIZE * SCALE);
				renderTile(renderer, tile, dest);
			}
		}
	}
}
