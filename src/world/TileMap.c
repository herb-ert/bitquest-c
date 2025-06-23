#include <world/TileMap.h>
#include <config.h>

TileMap createTileMap(int width, int height) {
	TileMap map;
	map.width = width;
	map.height = height;
	map.layerCount = LAYER_COUNT;

	for (int i = 0; i < LAYER_COUNT; i++) {
		map.layers[i] = createTileMapLayer(width, height);
	}

	return map;
}

void destroyTileMap(TileMap *map) {
	for (int i = 0; i < map->layerCount; i++) {
		destroyTileMapLayer(&map->layers[i]);
	}
}

void updateTileMap(TileMap *map) {
	for (int l = 0; l < map->layerCount; l++) {
		for (int y = 0; y < map->height; y++) {
			for (int x = 0; x < map->width; x++) {
				updateTile(&map->layers[l].tiles[y][x]);
			}
		}
	}
}

void renderTileMap(SDL_Renderer *renderer, TileMap *map) {
	for (int i = 0; i < map->layerCount; i++) {
		renderTileMapLayer(renderer, &map->layers[i]);
	}
}
