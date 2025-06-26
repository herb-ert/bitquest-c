#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <core/Tile.h>
#include <world/TileMap.h>
#include <entities/Player.h>
#include <config.h>
#include <core/Input.h>
#include <systems/Time.h>
#include <systems/Daylight.h>
#include <core/AssetManager.h>

int main(int argc, char* argv[])
{
  if (SDL_Init(SDL_INIT_VIDEO) != 0)
  {
    printf("SDL Init Error: %s\n", SDL_GetError());
    return 1;
  }

  if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
  {
    printf("SDL_image Init Error: %s\n", IMG_GetError());
    SDL_Quit();
    return 1;
  }

  SDL_Window* window = SDL_CreateWindow("BitQuest TileMap",
                                        SDL_WINDOWPOS_CENTERED,
                                        SDL_WINDOWPOS_CENTERED,
                                        TILE_SIZE * SCALE * MAP_WIDTH,
                                        TILE_SIZE * SCALE * MAP_HEIGHT,
                                        SDL_WINDOW_SHOWN);

  if (!window)
  {
    printf("Window creation failed: %s\n", SDL_GetError());
    IMG_Quit();
    SDL_Quit();
    return 1;
  }

  SDL_Renderer* renderer = SDL_CreateRenderer(window, -1,
                                              SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

  if (!renderer)
  {
    printf("Renderer creation failed: %s\n", SDL_GetError());
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
    return 1;
  }

  SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");

  // Asset manager
  AssetManager assets;
  initAssetManager(&assets, renderer);

  SDL_Texture* playerTex = loadTexture(&assets, "player", "../assets/player.png");
  SDL_Texture* grassTex = loadTexture(&assets, "grass", "../assets/tiles/grass.png");
  SDL_Texture* bladesOfGrassTex = loadTexture(&assets, "blades_of_grass", "../assets/tiles/blades_of_grass.png");
  SDL_Texture* groundTex = loadTexture(&assets, "ground", "../assets/tiles/ground.png");
  SDL_Texture* waterTex = loadTexture(&assets, "water", "../assets/tiles/water.png");
  SDL_Texture* wallTex = loadTexture(&assets, "wall", "../assets/tiles/wall.png");
  SDL_Texture* crateTex = loadTexture(&assets, "crate", "../assets/tiles/crate.png");
  SDL_Texture* signTex = loadTexture(&assets, "sign", "../assets/tiles/sign.png");
  SDL_Texture* chestTex = loadTexture(&assets, "chest", "../assets/tiles/chest.png");
  SDL_Texture* planksTex = loadTexture(&assets, "planks", "../assets/tiles/planks.png");

  if (!playerTex || !grassTex || !bladesOfGrassTex || !groundTex || !waterTex ||
    !wallTex || !crateTex || !signTex || !chestTex || !planksTex)
  {
    printf("Exiting due to texture load failure(s).\n");
    destroyAssetManager(&assets);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
    return 1;
  }

  // Collision boxes
#define FULL_COLLISION (CollisionBox){0, 0, 1, 1}
#define NO_COLLISION   (CollisionBox){0, 0, 0, 0}
#define HALF_BOTTOM    (CollisionBox){0, 0.5f, 1, 0.5f}

  Player player = createPlayer(playerTex, 1, 1);
  TileMap map = createTileMap(MAP_WIDTH, MAP_HEIGHT);

  // Fill ground layer
  for (int y = 0; y < MAP_HEIGHT; y++)
  {
    for (int x = 0; x < MAP_WIDTH; x++)
    {
      map.layers[LAYER_GROUND].tiles[y][x] = createTile(grassTex, false, false, 0, 0, 0.0f, NO_COLLISION);
    }
  }

  // Object tiles with collisions
  map.layers[LAYER_OBJECTS].tiles[6][4] = createTile(crateTex, false, false, 0, 0, 0.0f, FULL_COLLISION);
  map.layers[LAYER_OBJECTS].tiles[6][5] = createTile(crateTex, false, false, 0, 0, 0.0f, FULL_COLLISION);
  map.layers[LAYER_OBJECTS].tiles[8][5] = createTile(signTex, false, false, 0, 0, 0.0f, HALF_BOTTOM);
  map.layers[LAYER_OBJECTS].tiles[6][6] = createTile(chestTex, false, true, 0, 0, 0.15f, FULL_COLLISION);

  // Water with full collision
  for (int i = 6; i <= 8; i++)
  {
    map.layers[LAYER_GROUND].tiles[1][i] = createTile(waterTex, false, true, 0, 0, 0.5f, FULL_COLLISION);
  }

  // Planks - walkable
  for (int i = 0; i <= 8; i++)
  {
    map.layers[LAYER_GROUND].tiles[3][i] = createTile(planksTex, false, false, 0, 0, 0.0f, NO_COLLISION);
  }

  // Add blades of grass as decoration (non-collidable)
  for (int y = 0; y < MAP_HEIGHT; y++)
  {
    for (int x = 0; x < MAP_WIDTH; x++)
    {
      Tile* groundTile = &map.layers[LAYER_GROUND].tiles[y][x];
      if (groundTile->sprite.texture != grassTex) continue;

      bool occupied = false;
      for (int l = 0; l < map.layerCount; l++)
      {
        if (l == LAYER_GROUND) continue;
        if (map.layers[l].tiles[y][x].sprite.texture != NULL)
        {
          occupied = true;
          break;
        }
      }
      if (occupied) continue;

      if ((rand() % 100) < 55)
      {
        map.layers[LAYER_DECORATION].tiles[y][x] = createTile(bladesOfGrassTex, true, false, 0, 0, 0.0f, NO_COLLISION);
      }
    }
  }

  // Time & input
  TimeSystem gameTime;
  initTimeSystem(&gameTime, 1.0f / 60.0f);

  InputState input;
  resetInput(&input);

  Uint32 lastTime = SDL_GetTicks();
  float deltaTime = 0.0f;

  SDL_Event e;
  int quit = 0;

  while (!quit)
  {
    Uint32 currentTime = SDL_GetTicks();
    deltaTime = (currentTime - lastTime) / 1000.0f;
    lastTime = currentTime;

    while (SDL_PollEvent(&e))
    {
      if (e.type == SDL_QUIT)
        quit = 1;
      else if (e.type == SDL_KEYDOWN)
        handleKeyDown(&input, e.key.keysym.sym);
      else if (e.type == SDL_KEYUP)
        handleKeyUp(&input, e.key.keysym.sym);
    }

    float dx = 0.0f, dy = 0.0f;
    if (input.left && !input.right) dx = -1.0f;
    else if (input.right && !input.left) dx = 1.0f;
    if (input.up && !input.down) dy = -1.0f;
    else if (input.down && !input.up) dy = 1.0f;

    if (dx != 0.0f || dy != 0.0f)
      movePlayer(&player, &map, dx, dy, deltaTime);

    updateTileMap(&map, deltaTime);
    updateTimeSystem(&gameTime, deltaTime);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    int playerRow = (int)(player.y + 1);
    if (playerRow >= MAP_HEIGHT) playerRow = MAP_HEIGHT - 1;

    for (int y = 0; y < MAP_HEIGHT; y++)
    {
      for (int x = 0; x < MAP_WIDTH; x++)
      {
        SDL_Rect dest = {
          x * TILE_SIZE * SCALE,
          y * TILE_SIZE * SCALE,
          TILE_SIZE * SCALE,
          TILE_SIZE * SCALE
        };

        for (int layer = LAYER_GROUND; layer <= LAYER_DECORATION; ++layer)
        {
          Tile* tile = &map.layers[layer].tiles[y][x];
          if (tile->sprite.texture)
            renderTile(renderer, tile, dest);
        }
      }

      if (y == playerRow)
      {
        renderPlayer(renderer, &player);
      }

      for (int x = 0; x < MAP_WIDTH; x++)
      {
        SDL_Rect dest = {
          x * TILE_SIZE * SCALE,
          y * TILE_SIZE * SCALE,
          TILE_SIZE * SCALE,
          TILE_SIZE * SCALE
        };

        for (int layer = LAYER_OBJECTS; layer < map.layerCount; ++layer)
        {
          Tile* tile = &map.layers[layer].tiles[y][x];
          if (tile->sprite.texture)
            renderTile(renderer, tile, dest);
        }
      }
    }

    // Time tint overlay
    SDL_Color tint = getTimeTintColor(&gameTime);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, tint.r, tint.g, tint.b, tint.a);
    SDL_Rect overlay = {
      0, 0,
      TILE_SIZE * SCALE * MAP_WIDTH,
      TILE_SIZE * SCALE * MAP_HEIGHT
    };
    SDL_RenderFillRect(renderer, &overlay);

    SDL_RenderPresent(renderer);
  }

  destroyTileMap(&map);
  destroyAssetManager(&assets);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  IMG_Quit();
  SDL_Quit();

  return 0;
}
