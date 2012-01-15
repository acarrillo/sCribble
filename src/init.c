/*
 * SDL framework borrowed from http://www.parallelrealities.co.uk/
 */
#include "init.h"

extern void line(int sx, int sy, int ex, int ey, int r, int g, int b);

void init(char *title) {

  if (TTF_Init() == -1){
    exit(1);
  }

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
    SDL_FillRect(screen, &screen->clip_rect, SDL_MapRGB(screen->format, 255, 255, 255)); //Sets screen white

    /* initialize mouse coordinate struct */
    mouse.xcor = -1;
    mouse.ycor = -1;
    mouse.lastx = -1;
    mouse.lasty = -1;

    /* initialize tool size */
    tool_width = 2;

    /* initialize tool color */
    color.r = 0;
    color.g = 0;
    color.b = 0;

    //draw divider line
    line(0, 425, 650, 425, 0, 0, 0); //DIVIDER LINE

    //draw slider line
    line(300,462,400,462,0,0,0);

    tool_width = 5;
}

void cleanup() {
	SDL_Quit();
}
