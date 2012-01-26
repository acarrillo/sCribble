/*
 * Saves a file of the working canvas.
 */
#include "file.h"


/*
 * Attempts to save a .bmp of the canvas
 */
void saveImage() {
    SDL_Surface *screen_to_save;
    screen_to_save = SDL_DisplayFormat(screen);
    SDL_Rect bounds;

    bounds.x = BORDER_WIDTH;
    bounds.y = BORDER_WIDTH;
    bounds.w = 100;
    bounds.h = 100;
    /*bounds.w = screen->w - BORDER_WIDTH;*/
    /*bounds.h = screen->h - BORDER_WIDTH - (screen->h - 446); // See line 226 of graphics.c for magic number #FIXME*/

    SDL_BlitSurface(screen,  &bounds, screen_to_save, &bounds);

    if (SDL_SaveBMP(screen_to_save, "mypicture.bmp") < 0) {
        printf("Could not save canvas\n");
    }
    else {
        printf("Canvas saved successfully to 'mypicture.bmp'\n");
    }
    SDL_FreeSurface(screen_to_save);
}
