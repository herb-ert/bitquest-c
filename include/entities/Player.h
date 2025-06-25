#ifndef PLAYER_H
#define PLAYER_H

#include <SDL.h>
#include <core/Sprite.h>

typedef struct {
	float x, y;
	float speed;
	Sprite sprite;
} Player;

Player createPlayer(SDL_Texture *texture, int startX, int startY);
void movePlayer(Player *player, float dx, float dy, float deltaTime);
void updatePlayer(Player *player, float deltaTime);
void renderPlayer(SDL_Renderer *renderer, const Player *player);

#endif
