#include <string.h>
#include <stdio.h>
#include <SDL_ttf.h>
#include "systems/Chat.h"

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
    int baseY = screenHeight - 20;

    for (int i = chat->count - 1; i >= 0 && i >= chat->count - 6; --i)
    {
        SDL_Surface* surf = TTF_RenderText_Blended(font, chat->messages[i].text, white);
        SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surf);
        SDL_Rect dst = {10, baseY, surf->w, surf->h};
        SDL_RenderCopy(renderer, tex, NULL, &dst);
        baseY -= surf->h + 2;
        SDL_FreeSurface(surf);
        SDL_DestroyTexture(tex);
    }

    if (chat->active)
    {
        char buffer[MAX_MESSAGE_LENGTH + 2];
        snprintf(buffer, sizeof(buffer), "> %s", chat->currentInput);
        SDL_Surface* inputSurf = TTF_RenderText_Blended(font, buffer, white);
        SDL_Texture* inputTex = SDL_CreateTextureFromSurface(renderer, inputSurf);
        SDL_Rect dst = {10, screenHeight - 20, inputSurf->w, inputSurf->h};
        SDL_RenderCopy(renderer, inputTex, NULL, &dst);
        SDL_FreeSurface(inputSurf);
        SDL_DestroyTexture(inputTex);
    }
}
