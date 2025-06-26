#include "AssetManager.h"
#include <stdlib.h>
#include <string.h>
#include <SDL_image.h>

void initAssetManager(AssetManager* manager, SDL_Renderer* renderer)
{
  manager->textures = NULL;
  manager->count = 0;
  manager->capacity = 0;
  manager->renderer = renderer;
}

void destroyAssetManager(AssetManager* manager)
{
  for (int i = 0; i < manager->count; i++)
  {
    SDL_DestroyTexture(manager->textures[i].texture);
    free(manager->textures[i].key);
  }
  free(manager->textures);
  manager->textures = NULL;
  manager->count = 0;
  manager->capacity = 0;
}

SDL_Texture* loadTexture(AssetManager* manager, const char* key, const char* path)
{
  // Check if texture with this key already loaded
  for (int i = 0; i < manager->count; i++)
  {
    if (strcmp(manager->textures[i].key, key) == 0)
    {
      return manager->textures[i].texture;
    }
  }

  SDL_Texture* tex = IMG_LoadTexture(manager->renderer, path);
  if (!tex)
  {
    printf("Failed to load texture for key '%s' from path '%s': %s\n", key, path, IMG_GetError());
    return NULL;
  }

  // Expand capacity if needed
  if (manager->count >= manager->capacity)
  {
    manager->capacity = (manager->capacity == 0) ? 8 : manager->capacity * 2;
    TextureEntry* newTextures = realloc(manager->textures, manager->capacity * sizeof(TextureEntry));
    if (!newTextures)
    {
      SDL_DestroyTexture(tex);
      printf("Failed to realloc texture entries in AssetManager.\n");
      return NULL; // allocation failure
    }
    manager->textures = newTextures;
  }

  // Store new texture and copy key string
  char* keyCopy = strdup(key);
  if (!keyCopy)
  {
    SDL_DestroyTexture(tex);
    printf("Failed to allocate memory for key copy in AssetManager.\n");
    return NULL;
  }

  TextureEntry entry = {keyCopy, tex};
  manager->textures[manager->count++] = entry;

  return tex;
}

SDL_Texture* getTexture(AssetManager* manager, const char* key)
{
  for (int i = 0; i < manager->count; i++)
  {
    if (strcmp(manager->textures[i].key, key) == 0)
    {
      return manager->textures[i].texture;
    }
  }
  return NULL;
}
