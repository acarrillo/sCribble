/*
 * SDL framework borrowed from http://www.parallelrealities.co.uk/
 */
#include "init.h"

extern void line(int sx, int sy, int ex, int ey, int r, int g, int b);
extern void Status(char*, TTF_Font *font);
extern TTF_Font *loadFont(char* name, int size);
extern void closeFont(TTF_Font *font);
extern Circle circle;
extern int toolno;

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

    TTF_Font * font;
    font = loadFont("font/blackWolf.ttf", 16);
    Status("Welcome to sCribble!", font);

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
    color.id = 0; // Black

    //draw slider line
    line(300,462,400,462,0,0,0);

    tool_width = 5;

    circle.iter = 0;

    toolno = 0;

    closeFont(font);
}

void cleanup() {
	SDL_Quit();
}
