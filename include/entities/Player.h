#ifndef PLAYER_H
#define PLAYER_H

#include <SDL.h>
#include <core/Sprite.h>
#include <world/TileMap.h>

typedef struct
{
  float x, y;
  float speed;
  Sprite sprite;
  CollisionBox collisionBox;
} Player;

Player createPlayer(SDL_Texture* texture, int startX, int startY);
void movePlayer(Player* player, TileMap* map, float dx, float dy, float deltaTime);
void updatePlayer(Player* player, float deltaTime);
void renderPlayer(SDL_Renderer* renderer, const Player* player);
int getPlayerFeetY(const Player* player);

#endif
