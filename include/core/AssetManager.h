#ifndef ASSET_MANAGER_H
#define ASSET_MANAGER_H

#include <SDL.h>

typedef struct
{
  char* key;
  SDL_Texture* texture;
} TextureEntry;

typedef struct
{
  TextureEntry* textures;
  int count;
  int capacity;
  SDL_Renderer* renderer;
} AssetManager;

void initAssetManager(AssetManager* manager, SDL_Renderer* renderer);
void destroyAssetManager(AssetManager* manager);
SDL_Texture* loadTexture(AssetManager* manager, const char* key, const char* path);
SDL_Texture* getTexture(AssetManager* manager, const char* key);

#endif
