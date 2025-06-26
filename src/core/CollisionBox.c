#include <CollisionBox.h>
#include <config.h>
#include <SDL.h>

void renderCollisionBox(SDL_Renderer* renderer, float x, float y, CollisionBox box)
{
  if (box.w <= 0 || box.h <= 0)
    return;

  SDL_Rect cb = {
    (int)((x + box.x) * TILE_SIZE * SCALE),
    (int)((y + box.y) * TILE_SIZE * SCALE),
    (int)(box.w * TILE_SIZE * SCALE),
    (int)(box.h * TILE_SIZE * SCALE)
  };

  SDL_SetRenderDrawColor(renderer, 255, 0, 0, 80); // Translucent red
  SDL_RenderFillRect(renderer, &cb);
  SDL_SetRenderDrawColor(renderer, 255, 0, 0, 200);
  SDL_RenderDrawRect(renderer, &cb);
}
