#ifndef PLAYER_H
#define PLAYER_H

#include <SDL.h>
#include <core/Sprite.h>

typedef struct {
	int x, y;
	Sprite sprite;
} Player;

Player createPlayer(SDL_Texture *texture, int startX, int startY);

void movePlayer(Player *player, int dx, int dy, int mapWidth, int mapHeight);

void updatePlayer(Player *player);

void renderPlayer(SDL_Renderer *renderer, const Player *player);

#endif
