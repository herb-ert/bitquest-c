#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <SDL_ttf.h>
#include "systems/Chat.h"

#define CHAT_BG_COLOR (SDL_Color){0, 0, 0, 80}
#define CHAT_TEXT_COLOR (SDL_Color){255, 255, 255, 255}

#define CHAT_PADDING_X 8
#define CHAT_PADDING_Y 4
#define CHAT_MARGIN_X 10
#define CHAT_MARGIN_Y 10
#define MAX_VISIBLE_MESSAGES 6

#define CURSOR_BLINK_INTERVAL 500       // ms
#define CHAT_HIDE_DELAY_MS 10000        // ms before auto-hiding if not active

void initChat(Chat* chat)
{
    chat->count = 0;
    chat->inputLength = 0;
    chat->active = false;
    chat->lastMessageTime = 0;
    chat->currentInput[0] = '\0';
}

void handleChatEvent(Chat* chat, SDL_Event* e)
{
    if (e->type == SDL_TEXTINPUT && chat->inputLength < MAX_MESSAGE_LENGTH - 1)
    {
        strcat(chat->currentInput, e->text.text);
        chat->inputLength += strlen(e->text.text);
        chat->lastMessageTime = SDL_GetTicks(); // activity resets timer
    }
    else if (e->type == SDL_KEYDOWN)
    {
        SDL_Keycode key = e->key.keysym.sym;
        if (key == SDLK_BACKSPACE && chat->inputLength > 0)
        {
            chat->currentInput[--chat->inputLength] = '\0';
            chat->lastMessageTime = SDL_GetTicks();
        }
        else if (key == SDLK_RETURN && chat->inputLength > 0)
        {
            submitChatMessage(chat);
        }
        else if (key == SDLK_ESCAPE)
        {
            chat->active = false;
            chat->currentInput[0] = '\0';
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
    chat->lastMessageTime = SDL_GetTicks();
}

void renderChatMessages(Chat* chat, SDL_Renderer* renderer, TTF_Font* font, int screenHeight)
{
    Uint32 now = SDL_GetTicks();

    bool shouldRender =
        chat->active || (chat->count > 0 && (now - chat->lastMessageTime < CHAT_HIDE_DELAY_MS));

    if (!shouldRender)
        return;

    SDL_Color textColor = CHAT_TEXT_COLOR;

    char widestLine[MAX_MESSAGE_LENGTH + 1];
    memset(widestLine, 'W', MAX_MESSAGE_LENGTH);
    widestLine[MAX_MESSAGE_LENGTH] = '\0';

    int textW = 0, textH = 0;
    TTF_SizeText(font, widestLine, &textW, &textH);

    int fullWidth = textW + CHAT_PADDING_X * 2;
    int fullHeight = textH + CHAT_PADDING_Y * 2;

    int baseY = screenHeight - CHAT_MARGIN_Y - fullHeight;

    bool showCursor = chat->active && ((now / CURSOR_BLINK_INTERVAL) % 2 == 0);

    if (chat->active)
    {
        char buffer[MAX_MESSAGE_LENGTH + 4];
        snprintf(buffer, sizeof(buffer), "%s%s", chat->currentInput, showCursor ? "|" : " ");

        SDL_Surface* inputSurf = TTF_RenderText_Blended(font, buffer, textColor);
        SDL_Texture* inputTex = SDL_CreateTextureFromSurface(renderer, inputSurf);

        SDL_Rect bg = {CHAT_MARGIN_X, baseY, fullWidth, fullHeight};
        SDL_SetRenderDrawColor(renderer, CHAT_BG_COLOR.r, CHAT_BG_COLOR.g, CHAT_BG_COLOR.b, CHAT_BG_COLOR.a);
        SDL_RenderFillRect(renderer, &bg);

        SDL_Rect textDst = {
            bg.x + CHAT_PADDING_X,
            bg.y + CHAT_PADDING_Y,
            inputSurf->w,
            inputSurf->h
        };
        SDL_RenderCopy(renderer, inputTex, NULL, &textDst);

        SDL_FreeSurface(inputSurf);
        SDL_DestroyTexture(inputTex);

        baseY -= fullHeight;
    }

    for (int i = chat->count - 1, shown = 0; i >= 0 && shown < MAX_VISIBLE_MESSAGES; --i, ++shown)
    {
        SDL_Surface* msgSurf = TTF_RenderText_Blended(font, chat->messages[i].text, textColor);
        SDL_Texture* msgTex = SDL_CreateTextureFromSurface(renderer, msgSurf);

        SDL_Rect bg = {CHAT_MARGIN_X, baseY, fullWidth, fullHeight};
        SDL_SetRenderDrawColor(renderer, CHAT_BG_COLOR.r, CHAT_BG_COLOR.g, CHAT_BG_COLOR.b, CHAT_BG_COLOR.a);
        SDL_RenderFillRect(renderer, &bg);

        SDL_Rect textDst = {
            bg.x + CHAT_PADDING_X,
            bg.y + CHAT_PADDING_Y,
            msgSurf->w,
            msgSurf->h
        };
        SDL_RenderCopy(renderer, msgTex, NULL, &textDst);

        baseY -= fullHeight;

        SDL_FreeSurface(msgSurf);
        SDL_DestroyTexture(msgTex);
    }
}
