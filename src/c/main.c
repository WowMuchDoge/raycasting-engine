#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#define WIDTH 640
#define HEIGHT 480

typedef struct {
	SDL_Window* window;
	SDL_Renderer* renderer;
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

	return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event) {
	if (event->type == SDL_EVENT_QUIT) {
		return SDL_APP_SUCCESS;
	}

	return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void* appstate) {

	return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void* appstate, SDL_AppResult resuilt) {
	SDL_free(appstate);
}
