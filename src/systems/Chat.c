#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <SDL_ttf.h>
#include "systems/Chat.h"

#define CHAT_BG_COLOR       (SDL_Color){0, 0, 0, 80}
#define CHAT_TEXT_COLOR     (SDL_Color){255, 255, 255, 255}
#define CHAT_HIGHLIGHT_COLOR (SDL_Color){255, 255, 0, 255}

#define CHAT_PADDING_X 8
#define CHAT_PADDING_Y 4
#define CHAT_MARGIN_X 10
#define CHAT_MARGIN_Y 10
#define MAX_VISIBLE_MESSAGES 6

#define CURSOR_BLINK_INTERVAL 500
#define CHAT_HIDE_DELAY_MS 2000

void initChat(Chat* chat)
{
    chat->count = 0;
    chat->inputLength = 0;
    chat->active = false;
    chat->lastMessageTime = 0;
    chat->scrollOffset = 0;
    chat->searching = false;
    chat->currentInput[0] = '\0';
}

void handleChatEvent(Chat* chat, SDL_Event* e)
{
    if (e->type == SDL_TEXTINPUT && chat->inputLength < MAX_MESSAGE_LENGTH - 1)
    {
        if (strcmp(e->text.text, "\t") != 0) // Ignore tab character from SDL_TEXTINPUT
        {
            strcat(chat->currentInput, e->text.text);
            chat->inputLength += strlen(e->text.text);
            chat->lastMessageTime = SDL_GetTicks();
        }
    }
    else if (e->type == SDL_KEYDOWN)
    {
        SDL_Keycode key = e->key.keysym.sym;

        if (key == SDLK_BACKSPACE && chat->inputLength > 0)
        {
            chat->currentInput[--chat->inputLength] = '\0';
            chat->lastMessageTime = SDL_GetTicks();
        }
        else if (key == SDLK_RETURN)
        {
            if (chat->searching)
            {
                chat->searching = false;
                chat->active = false;
                chat->inputLength = 0;
                chat->currentInput[0] = '\0';
            }
            else if (chat->inputLength > 0)
            {
                submitChatMessage(chat);
            }
        }
        else if (key == SDLK_ESCAPE)
        {
            chat->active = false;
            chat->searching = false;
            chat->currentInput[0] = '\0';
            chat->inputLength = 0;
        }
        else if (key == SDLK_UP)
        {
            if (chat->scrollOffset + MAX_VISIBLE_MESSAGES < chat->count)
                chat->scrollOffset++;
        }
        else if (key == SDLK_DOWN)
        {
            if (chat->scrollOffset > 0)
                chat->scrollOffset--;
        }
        else if (key == SDLK_TAB)
        {
            chat->searching = true;
            chat->active = true;
            chat->inputLength = 0;
            chat->currentInput[0] = '\0';
        }
    }
    else if (e->type == SDL_MOUSEWHEEL)
    {
        if (e->wheel.y > 0 && chat->scrollOffset + MAX_VISIBLE_MESSAGES < chat->count)
            chat->scrollOffset++;
        else if (e->wheel.y < 0 && chat->scrollOffset > 0)
            chat->scrollOffset--;
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
    chat->inputLength = 0;
    chat->currentInput[0] = '\0';
    chat->scrollOffset = 0;
    chat->active = false;
    chat->lastMessageTime = SDL_GetTicks();
}

bool matchesSearch(const char* query, const char* msg)
{
    if (!*query) return true;

    for (; *msg; ++msg)
    {
        const char* h = msg;
        const char* n = query;

        while (*h && *n && tolower((unsigned char)*h) == tolower((unsigned char)*n))
        {
            ++h;
            ++n;
        }

        if (!*n)
            return true;
    }

    return false;
}

void renderHighlightedText(SDL_Renderer* renderer, TTF_Font* font, const char* msg, const char* query, int x, int y)
{
    const char* match = NULL;
    size_t queryLen = strlen(query);

    for (const char* p = msg; *p; ++p)
    {
        size_t i = 0;
        while (p[i] && query[i] &&
            tolower((unsigned char)p[i]) == tolower((unsigned char)query[i]))
        {
            ++i;
        }
        if (i == queryLen)
        {
            match = p;
            break;
        }
    }

    if (!match || queryLen == 0)
    {
        SDL_Surface* surf = TTF_RenderText_Blended(font, msg, CHAT_TEXT_COLOR);
        SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surf);
        SDL_Rect dst = {x, y, surf->w, surf->h};
        SDL_RenderCopy(renderer, tex, NULL, &dst);
        SDL_FreeSurface(surf);
        SDL_DestroyTexture(tex);
        return;
    }

    char before[256], matchPart[256], after[256];
    int prefixLen = match - msg;

    strncpy(before, msg, prefixLen);
    before[prefixLen] = '\0';

    strncpy(matchPart, match, queryLen);
    matchPart[queryLen] = '\0';

    strcpy(after, match + queryLen);

    int dx = x;

    if (before[0])
    {
        SDL_Surface* surf = TTF_RenderText_Blended(font, before, CHAT_TEXT_COLOR);
        SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surf);
        SDL_Rect dst = {dx, y, surf->w, surf->h};
        SDL_RenderCopy(renderer, tex, NULL, &dst);
        dx += surf->w;
        SDL_FreeSurface(surf);
        SDL_DestroyTexture(tex);
    }

    SDL_Surface* surf = TTF_RenderText_Blended(font, matchPart, CHAT_HIGHLIGHT_COLOR);
    SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surf);
    SDL_Rect dst = {dx, y, surf->w, surf->h};
    SDL_RenderCopy(renderer, tex, NULL, &dst);
    dx += surf->w;
    SDL_FreeSurface(surf);
    SDL_DestroyTexture(tex);

    if (after[0])
    {
        surf = TTF_RenderText_Blended(font, after, CHAT_TEXT_COLOR);
        tex = SDL_CreateTextureFromSurface(renderer, surf);
        dst = (SDL_Rect){dx, y, surf->w, surf->h};
        SDL_RenderCopy(renderer, tex, NULL, &dst);
        SDL_FreeSurface(surf);
        SDL_DestroyTexture(tex);
    }
}

void renderChatMessages(Chat* chat, SDL_Renderer* renderer, TTF_Font* font, int screenHeight)
{
    Uint32 now = SDL_GetTicks();

    bool shouldRender = chat->active || (chat->count > 0 && (now - chat->lastMessageTime < CHAT_HIDE_DELAY_MS));
    if (!shouldRender) return;

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

        SDL_Surface* inputSurf = TTF_RenderText_Blended(font, buffer, CHAT_TEXT_COLOR);
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

    int shown = 0;
    for (int i = chat->count - 1 - chat->scrollOffset; i >= 0 && shown < MAX_VISIBLE_MESSAGES; --i)
    {
        if (chat->searching && !matchesSearch(chat->currentInput, chat->messages[i].text))
            continue;

        SDL_Rect bg = {CHAT_MARGIN_X, baseY, fullWidth, fullHeight};
        SDL_SetRenderDrawColor(renderer, CHAT_BG_COLOR.r, CHAT_BG_COLOR.g, CHAT_BG_COLOR.b, CHAT_BG_COLOR.a);
        SDL_RenderFillRect(renderer, &bg);

        int textX = bg.x + CHAT_PADDING_X;
        int textY = bg.y + CHAT_PADDING_Y;

        if (chat->searching)
            renderHighlightedText(renderer, font, chat->messages[i].text, chat->currentInput, textX, textY);
        else
        {
            SDL_Surface* msgSurf = TTF_RenderText_Blended(font, chat->messages[i].text, CHAT_TEXT_COLOR);
            SDL_Texture* msgTex = SDL_CreateTextureFromSurface(renderer, msgSurf);
            SDL_Rect textDst = {textX, textY, msgSurf->w, msgSurf->h};
            SDL_RenderCopy(renderer, msgTex, NULL, &textDst);
            SDL_FreeSurface(msgSurf);
            SDL_DestroyTexture(msgTex);
        }

        baseY -= fullHeight;
        shown++;
    }
}
