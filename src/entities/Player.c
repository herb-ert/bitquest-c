#include <entities/Player.h>
#include <world/TileMap.h>
#include "config.h"
#include <math.h>
#include <CollisionBox.h>

#define PLAYER_WIDTH  1.0f
#define PLAYER_HEIGHT 1.0f
#define PLAYER_COLLISION_BOX (CollisionBox){0.0f, 0.5f, 1.0f, 0.5f}

Player createPlayer(SDL_Texture* texture, int startX, int startY)
{
  Sprite sprite = createSprite(texture, false, false, 0, 0, 0);
  Player p = {
    .x = (float)startX,
    .y = (float)startY,
    .speed = 5.0f,
    .sprite = sprite,
    .collisionBox = PLAYER_COLLISION_BOX
  };
  return p;
}

bool aabbIntersect(float ax, float ay, float aw, float ah,
                   float bx, float by, float bw, float bh)
{
  return !(bx >= ax + aw ||
    bx + bw <= ax ||
    by >= ay + ah ||
    by + bh <= ay);
}

bool isBlocked(TileMap* map, Player* player)
{
  float playerColX = player->x + player->collisionBox.x - 0.5f;
  float playerColY = player->y + player->collisionBox.y - 0.5f;
  float playerColW = player->collisionBox.w;
  float playerColH = player->collisionBox.h;

  int tileLeft = (int)floorf(playerColX);
  int tileRight = (int)floorf(playerColX + playerColW);
  int tileTop = (int)floorf(playerColY);
  int tileBottom = (int)floorf(playerColY + playerColH);

  for (int ty = tileTop; ty <= tileBottom; ++ty)
  {
    for (int tx = tileLeft; tx <= tileRight; ++tx)
    {
      if (tx < 0 || tx >= MAP_WIDTH || ty < 0 || ty >= MAP_HEIGHT)
        return true;

      for (int layer = 0; layer < map->layerCount; ++layer)
      {
        Tile* tile = &map->layers[layer].tiles[ty][tx];
        if (tile->sprite.texture && tile->collisionBox.w > 0 && tile->collisionBox.h > 0)
        {
          float tileColX = tx + tile->collisionBox.x;
          float tileColY = ty + tile->collisionBox.y;
          float tileColW = tile->collisionBox.w;
          float tileColH = tile->collisionBox.h;

          if (aabbIntersect(playerColX, playerColY, playerColW, playerColH,
                            tileColX, tileColY, tileColW, tileColH))
          {
            return true;
          }
        }
      }
    }
  }

  return false;
}

void movePlayer(Player* player, TileMap* map, float dx, float dy, float deltaTime)
{
  float tryX = player->x + dx * player->speed * deltaTime;
  float tryY = player->y;

  Player tryPlayer = *player;
  tryPlayer.x = tryX;
  if (!isBlocked(map, &tryPlayer))
    player->x = tryX;

  tryPlayer.x = player->x;
  tryPlayer.y = player->y + dy * player->speed * deltaTime;
  if (!isBlocked(map, &tryPlayer))
    player->y = tryPlayer.y;

  if (player->x < PLAYER_WIDTH / 2.0f)
    player->x = PLAYER_WIDTH / 2.0f;
  if (player->x > MAP_WIDTH - PLAYER_WIDTH / 2.0f)
    player->x = MAP_WIDTH - PLAYER_WIDTH / 2.0f;

  if (player->y < PLAYER_HEIGHT / 2.0f)
    player->y = PLAYER_HEIGHT / 2.0f;
  if (player->y > MAP_HEIGHT - PLAYER_HEIGHT / 2.0f)
    player->y = MAP_HEIGHT - PLAYER_HEIGHT / 2.0f;
}

void updatePlayer(Player* player, float deltaTime)
{
  updateSprite(&player->sprite, deltaTime);
}

void renderPlayer(SDL_Renderer* renderer, const Player* player)
{
  SDL_Rect dest = {
    (int)((player->x - 0.5f) * TILE_SIZE * SCALE),
    (int)((player->y - 0.5f) * TILE_SIZE * SCALE),
    TILE_SIZE * SCALE,
    TILE_SIZE * SCALE
  };
  renderSprite(renderer, &player->sprite, dest);
  renderCollisionBox(renderer, player->x - 0.5f, player->y - 0.5f, player->collisionBox);
}
