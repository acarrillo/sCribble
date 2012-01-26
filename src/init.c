/**
 * init.c
 * Copyright (c) 2012 Alejandro Carrillo, Doron Shapiro, and Vincent Zhen
 *
 * This file is part of sCribble.
 *
 * sCribble is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * sCribble is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with sCribble.  If not, see <http://www.gnu.org/licenses/>.
 */

/*
 * SDL framework borrowed from http://www.parallelrealities.co.uk/
 */
#include "init.h"
#include <signal.h>

extern void line(int sx, int sy, int ex, int ey, int r, int g, int b);
extern void drawToolsBackground();
extern void setImages();
extern void Status(char*, TTF_Font *font);
extern TTF_Font *loadFont(char* name, int size);
extern void closeFont(TTF_Font *font);
extern Circle circle;
extern int toolno;
extern int local_tool_width;

extern void saveImage(); // from file.c

extern void cleanup_client(); // From client.c
extern void initClient(char*); // From client.c

void init(char *title, char *addr) {
    // Initializes network communications
    initClient(addr); //TODO: Test external servers

    if (TTF_Init() == -1){
        exit(1);
    }

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("Could not initialize SDL: %s\n", SDL_GetError());
		exit(1);
	}

    SDL_WM_SetIcon(SDL_LoadBMP("gfx/color_line.bmp"), NULL);
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

    //Draws the background
    drawToolsBackground();
    setImages();

    //draw slider line
    line(300,462,400,462,0,0,0);

    tool_width = 5;

    circle.iter = 0;

    toolno = 0;
    local_tool_width = 5; // Initializes tool to have a stroke of 5
    closeFont(font);
}

void cleanup() {
  cleanup_client(); // Disconnect from server
  saveImage();      // Save canvas as .bmp
  SDL_Quit();       // Tear down all the SDL things
  printf("killing server\n");
  kill(0, 2);
}
