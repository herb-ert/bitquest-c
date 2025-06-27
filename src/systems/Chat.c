#include <string.h>
#include <stdio.h>
#include <SDL_ttf.h>
#include "systems/Chat.h"

#define CHAT_BG_COLOR (SDL_Color){0, 0, 0, 80}

void initChat(Chat* chat)
{
    chat->count = 0;
    chat->inputLength = 0;
    chat->active = false;
    chat->currentInput[0] = '\0';
}

void handleChatEvent(Chat* chat, SDL_Event* e)
{
    if (e->type == SDL_TEXTINPUT && chat->inputLength < MAX_MESSAGE_LENGTH - 1)
    {
        strcat(chat->currentInput, e->text.text);
        chat->inputLength += strlen(e->text.text);
    }
    else if (e->type == SDL_KEYDOWN)
    {
        SDL_Keycode key = e->key.keysym.sym;
        if (key == SDLK_BACKSPACE && chat->inputLength > 0)
        {
            chat->currentInput[--chat->inputLength] = '\0';
        }
        else if (key == SDLK_RETURN && chat->inputLength > 0)
        {
            submitChatMessage(chat);
        }
        else if (key == SDLK_ESCAPE)
        {
            chat->active = false;
        }
    }
}

void submitChatMessage(Chat* chat)
{
    if (chat->count >= MAX_CHAT_MESSAGES)
    {
        memmove(&chat->messages[0], &chat->messages[1], sizeof(ChatMessage) * (MAX_CHAT_MESSAGES - 1));
        chat->count = MAX_CHAT_MESSAGES - 1;
    }
    strcpy(chat->messages[chat->count++].text, chat->currentInput);
    chat->currentInput[0] = '\0';
    chat->inputLength = 0;
    chat->active = false;
}

void renderChatMessages(Chat* chat, SDL_Renderer* renderer, TTF_Font* font, int screenHeight)
{
    SDL_Color white = {255, 255, 255, 255};
    int baseY = screenHeight - 40;

    // Precompute max width for consistent background sizing
    char widestLine[MAX_MESSAGE_LENGTH + 1];
    memset(widestLine, 'W', MAX_MESSAGE_LENGTH);
    widestLine[MAX_MESSAGE_LENGTH] = '\0';

    int maxWidth = 0, maxHeight = 0;
    TTF_SizeText(font, widestLine, &maxWidth, &maxHeight);

    for (int i = chat->count - 1; i >= 0 && i >= chat->count - 6; --i)
    {
        SDL_Surface* surf = TTF_RenderText_Blended(font, chat->messages[i].text, white);
        SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surf);

        SDL_Rect bg = {10, baseY, maxWidth, surf->h};

        // Draw background
        SDL_SetRenderDrawColor(renderer, CHAT_BG_COLOR.r, CHAT_BG_COLOR.g, CHAT_BG_COLOR.b, CHAT_BG_COLOR.a);
        SDL_RenderFillRect(renderer, &bg);

        // Draw text
        SDL_Rect textDst = {10, baseY, surf->w, surf->h};
        SDL_RenderCopy(renderer, tex, NULL, &textDst);

        baseY -= surf->h;

        SDL_FreeSurface(surf);
        SDL_DestroyTexture(tex);
    }

    if (chat->active)
    {
        char buffer[MAX_MESSAGE_LENGTH + 3];
        snprintf(buffer, sizeof(buffer), ">%s", chat->currentInput);

        SDL_Surface* inputSurf = TTF_RenderText_Blended(font, buffer, white);
        SDL_Texture* inputTex = SDL_CreateTextureFromSurface(renderer, inputSurf);

        SDL_Rect bg = {10, screenHeight - 20, maxWidth, inputSurf->h};

        SDL_SetRenderDrawColor(renderer, CHAT_BG_COLOR.r, CHAT_BG_COLOR.g, CHAT_BG_COLOR.b, CHAT_BG_COLOR.a);
        SDL_RenderFillRect(renderer, &bg);

        SDL_Rect textDst = {10, screenHeight - 20, inputSurf->w, inputSurf->h};
        SDL_RenderCopy(renderer, inputTex, NULL, &textDst);

        SDL_FreeSurface(inputSurf);
        SDL_DestroyTexture(inputTex);
    }
}
