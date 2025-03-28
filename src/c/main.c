#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include "stdio.h"

#include "player.h"

#define WIDTH 640
#define HEIGHT 480

#define PIXELS_PER_METER 100.0

typedef struct {
	SDL_Window* window;
	SDL_Renderer* renderer;
	Player player;
} AppState;

SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[]) {
	SDL_SetAppMetadata("raycasting-engine", "0.1", "com.wowmuchdoge.raycasting-engine");

	AppState* as = SDL_malloc(sizeof(AppState));

	if (!SDL_Init(SDL_INIT_VIDEO)) {
		SDL_Log("Could not initialize video: %s", SDL_GetError());
		return SDL_APP_FAILURE;
	}

	if (!SDL_CreateWindowAndRenderer("raycasting-engine", WIDTH, HEIGHT, 0, &as->window, &as->renderer)) {
		SDL_Log("Could not create window and/or renderer: %s", SDL_GetError());
		return SDL_APP_FAILURE;
	}

	as->player = CreatePlayer(0.0, 0.0, 0.0);

	*appstate = as;

	return SDL_APP_CONTINUE;
}

Speeds PlayerMovement(SDL_Event* event, Speeds currentSpeeds) {
	// We do this for several reasons. First, if the player hits a key
	// that isn't here, we want the player to be unaffected. Second,
	// if a player is holding down a key while still pressing another,
	// we want to leave that speed be and only change what the event should
	// be modifying
	Speeds newSpeeds = currentSpeeds;

	if (event->type == SDL_EVENT_KEY_DOWN) {
		switch (event->key.scancode) {
			case SDL_SCANCODE_W:
				newSpeeds.vy = 2.0;
				break;
			case SDL_SCANCODE_S:
				newSpeeds.vy = -2.0;
				break;
			case SDL_SCANCODE_A:
				// Remember this is radians per second
				newSpeeds.omega = -3.0;
				break;
			case SDL_SCANCODE_D:
				newSpeeds.omega = 3.0;
				break;
			default:
				break;
		}
	} else if (event->type == SDL_EVENT_KEY_UP) {	
		switch (event->key.scancode) {
			case SDL_SCANCODE_W:
			case SDL_SCANCODE_S:
				newSpeeds.vy = 0.0;
				break;
			case SDL_SCANCODE_A:
			case SDL_SCANCODE_D:
				newSpeeds.omega = 0.0;
				break;
			default:
				break;
		}
	}

	return newSpeeds;
}

SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event) {

	AppState* as = (AppState*)(appstate);

	if (event->type == SDL_EVENT_QUIT) {
		return SDL_APP_SUCCESS;
	}

	Speeds playerRelativeSpeeds = PlayerMovement(event, as->player.currentSpeeds);

	SDL_Log("pvx %f, pvy %f", playerRelativeSpeeds.vx, playerRelativeSpeeds.vy);

	SetSpeeds(playerRelativeSpeeds, &as->player);

	return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void* appstate) {

	AppState* as = (AppState*)appstate;

	IteratePlayer(&as->player);

	SDL_Log("Player position x %f, y %f, vx %f, vy %f\n", as->player.x, as->player.y,
			as->player.currentSpeeds.vx, as->player.currentSpeeds.vy);

	SDL_FRect player = {as->player.x * PIXELS_PER_METER, as->player.y * PIXELS_PER_METER, 50, 50};

	// SDL_RenderClear sets the entire screen to the render draw color
	SDL_SetRenderDrawColor(as->renderer, 0, 0, 0, 255);
	SDL_RenderClear(as->renderer);

	SDL_SetRenderDrawColor(as->renderer, 23, 34, 249, 128);
	SDL_RenderFillRect(as->renderer, &player);
	SDL_RenderPresent(as->renderer);

	return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void* appstate, SDL_AppResult resuilt) {
	SDL_free(appstate);
}
