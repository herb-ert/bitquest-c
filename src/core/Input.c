#include <core/Input.h>

void resetInput(InputState *input) {
	input->up = false;
	input->down = false;
	input->left = false;
	input->right = false;
	input->interact = false;
	input->pause = false;
}

void handleKeyDown(InputState *input, SDL_Keycode key) {
	switch (key) {
		case SDLK_w:
		case SDLK_UP: input->up = true;
			break;
		case SDLK_s:
		case SDLK_DOWN: input->down = true;
			break;
		case SDLK_a:
		case SDLK_LEFT: input->left = true;
			break;
		case SDLK_d:
		case SDLK_RIGHT: input->right = true;
			break;
		case SDLK_e: input->interact = true;
			break;
		case SDLK_ESCAPE: input->pause = true;
			break;
		default: break;
	}
}

void handleKeyUp(InputState *input, SDL_Keycode key) {
	switch (key) {
		case SDLK_w:
		case SDLK_UP: input->up = false;
			break;
		case SDLK_s:
		case SDLK_DOWN: input->down = false;
			break;
		case SDLK_a:
		case SDLK_LEFT: input->left = false;
			break;
		case SDLK_d:
		case SDLK_RIGHT: input->right = false;
			break;
		case SDLK_e: input->interact = false;
			break;
		case SDLK_ESCAPE: input->pause = false;
			break;
		default: break;
	}
}
