#include <entities/Player.h>
#include "config.h"

Player createPlayer(SDL_Texture *texture, int startX, int startY) {
	Sprite sprite = createSprite(texture, false, false, 0, 0, 0);
	Player p = {(float) startX, (float) startY, 5.0f, sprite};
	return p;
}

void movePlayer(Player *player, float dx, float dy, float deltaTime) {
	player->x += dx * player->speed * deltaTime;
	player->y += dy * player->speed * deltaTime;

	if (player->x < 0) player->x = 0;
	if (player->x > MAP_WIDTH - 1) player->x = MAP_WIDTH - 1;

	if (player->y < 0) player->y = 0;
	if (player->y > MAP_HEIGHT - 1) player->y = MAP_HEIGHT - 1;
}

void updatePlayer(Player *player, float deltaTime) {
	updateSprite(&player->sprite, deltaTime);
}

void renderPlayer(SDL_Renderer *renderer, const Player *player) {
	SDL_Rect dest = {
		(int) (player->x * TILE_SIZE * SCALE),
		(int) (player->y * TILE_SIZE * SCALE),
		TILE_SIZE * SCALE,
		TILE_SIZE * SCALE
	};
	renderSprite(renderer, &player->sprite, dest);
}
