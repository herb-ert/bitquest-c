#ifndef CHAT_H
#define CHAT_H

#include <SDL.h>
#include <SDL_ttf.h>

#define MAX_CHAT_MESSAGES 50
#define MAX_MESSAGE_LENGTH 32

typedef struct
{
  char text[MAX_MESSAGE_LENGTH];
} ChatMessage;

typedef struct
{
  ChatMessage messages[MAX_CHAT_MESSAGES];
  int count;
  char currentInput[MAX_MESSAGE_LENGTH];
  int inputLength;
  bool active;
  Uint32 lastMessageTime;
} Chat;

void initChat(Chat* chat);
void handleChatEvent(Chat* chat, SDL_Event* e);
void submitChatMessage(Chat* chat);
void renderChatMessages(Chat* chat, SDL_Renderer* renderer, TTF_Font* font, int screenHeight);

#endif
