#include <entities/Player.h>
#include "config.h"

Player createPlayer(SDL_Texture *texture, int startX, int startY) {
	Sprite sprite = createSprite(texture, false, false, 0, 0, 0); // No variants or animation for now
	Player p = {startX, startY, sprite};
	return p;
}

void movePlayer(Player *player, int dx, int dy, int mapWidth, int mapHeight) {
	int newX = player->x + dx;
	int newY = player->y + dy;

	if (newX >= 0 && newX < mapWidth && newY >= 0 && newY < mapHeight) {
		player->x = newX;
		player->y = newY;
	}
}

void updatePlayer(Player *player) {
	updateSprite(&player->sprite);
}

void renderPlayer(SDL_Renderer *renderer, const Player *player) {
	SDL_Rect dest = {
		player->x * TILE_SIZE * SCALE,
		player->y * TILE_SIZE * SCALE,
		TILE_SIZE * SCALE,
		TILE_SIZE * SCALE
	};
	renderSprite(renderer, &player->sprite, dest);
}
