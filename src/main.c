#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <core/Tile.h>
#include <world/TileMap.h>
#include <entities/Player.h>
#include <config.h>
#include <core/Input.h>

int main(int argc, char *argv[]) {
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		printf("SDL Init Error: %s\n", SDL_GetError());
		return 1;
	}

	if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
		printf("SDL_image Init Error: %s\n", IMG_GetError());
		SDL_Quit();
		return 1;
	}

	SDL_Window *window = SDL_CreateWindow(
		"BitQuest TileMap",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		(TILE_SIZE * SCALE) * MAP_WIDTH, (TILE_SIZE * SCALE) * MAP_HEIGHT, SDL_WINDOW_SHOWN
	);
	if (!window) {
		printf("Window creation failed: %s\n", SDL_GetError());
		IMG_Quit();
		SDL_Quit();
		return 1;
	}

	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (!renderer) {
		printf("Renderer creation failed: %s\n", SDL_GetError());
		SDL_DestroyWindow(window);
		IMG_Quit();
		SDL_Quit();
		return 1;
	}

	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");

	// Load textures
	SDL_Texture *playerTex = IMG_LoadTexture(renderer, "../assets/player.png");

	SDL_Texture *grassTex = IMG_LoadTexture(renderer, "../assets/tiles/grass.png");
	SDL_Texture *groundTex = IMG_LoadTexture(renderer, "../assets/tiles/ground.png");
	SDL_Texture *waterTex = IMG_LoadTexture(renderer, "../assets/tiles/water.png");
	SDL_Texture *wallTex = IMG_LoadTexture(renderer, "../assets/tiles/wall.png");
	SDL_Texture *crateTex = IMG_LoadTexture(renderer, "../assets/tiles/crate.png");
	SDL_Texture *signTex = IMG_LoadTexture(renderer, "../assets/tiles/sign.png");

	if (!grassTex || !groundTex || !waterTex || !wallTex || !crateTex || !signTex || !playerTex) {
		printf("Failed to load one or more textures: %s\n", IMG_GetError());
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		IMG_Quit();
		SDL_Quit();
		return 1;
	}

	// Create the player
	Player player = createPlayer(playerTex, 1, 1);

	// Create static/shared tiles
	Tile groundTile = createTile(groundTex, false, false, 0, 0, 0);
	Tile wallTile = createTile(wallTex, false, false, 0, 0, 0);
	Tile crateTile = createTile(crateTex, false, false, 0, 0, 0);
	Tile signTile = createTile(signTex, false, false, 0, 0, 0);

	// Create and populate tilemap
	TileMap map = createTileMap(MAP_WIDTH, MAP_HEIGHT);

	for (int y = 0; y < MAP_HEIGHT; y++) {
		for (int x = 0; x < MAP_WIDTH; x++) {
			map.layers[LAYER_BACKGROUND].tiles[y][x] = createTile(grassTex, true, false, 0, 0, 0);
		}
	}

	map.layers[LAYER_DECORATION].tiles[2][3] = crateTile;
	map.layers[LAYER_DECORATION].tiles[3][4] = crateTile;
	map.layers[LAYER_DECORATION].tiles[5][5] = signTile;

	for (int i = 6; i <= 8; i++) {
		map.layers[LAYER_DECORATION].tiles[1][i] = createTile(waterTex, false, true, 0, 0, 500);
	}

	// Input
	InputState input;
	resetInput(&input);

	// Game loop
	SDL_Event e;
	int quit = 0;

	while (!quit) {
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT) {
				quit = 1;
			}
			else if (e.type == SDL_KEYDOWN) {
				handleKeyDown(&input, e.key.keysym.sym);
				// Move instantly on keydown
				if (input.up) movePlayer(&player, 0, -1, MAP_WIDTH, MAP_HEIGHT);
				if (input.down) movePlayer(&player, 0, 1, MAP_WIDTH, MAP_HEIGHT);
				if (input.left) movePlayer(&player, -1, 0, MAP_WIDTH, MAP_HEIGHT);
				if (input.right) movePlayer(&player, 1, 0, MAP_WIDTH, MAP_HEIGHT);
			}
			else if (e.type == SDL_KEYUP) {
				handleKeyUp(&input, e.key.keysym.sym);
			}
		}

		updateTileMap(&map);

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		renderTileMap(renderer, &map);
		renderPlayer(renderer, &player);

		SDL_RenderPresent(renderer);
	}

	// Cleanup
	destroyTileMap(&map);

	SDL_DestroyTexture(playerTex);
	SDL_DestroyTexture(grassTex);
	SDL_DestroyTexture(groundTex);
	SDL_DestroyTexture(waterTex);
	SDL_DestroyTexture(wallTex);
	SDL_DestroyTexture(crateTex);
	SDL_DestroyTexture(signTex);

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	IMG_Quit();
	SDL_Quit();

	return 0;
}
