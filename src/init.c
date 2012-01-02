/*
 * SDL framework borrowed from http://www.parallelrealities.co.uk/
 */
#include "init.h"

void init(char *title) {

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("Could not initialize SDL: %s\n", SDL_GetError());
		exit(1);
	}
	
    screen = SDL_SetVideoMode(640, 480, 0, SDL_HWSURFACE);
	
	if (screen == NULL) {
		printf("Couldn't set screen mode to 640 x 480: %s\n", SDL_GetError());

		exit(1);
	}
	
	
	SDL_WM_SetCaption(title, NULL); // Sets screen title
}

void cleanup() {
	SDL_Quit();
}
