/*
 * SDL framework borrowed from http://www.parallelrealities.co.uk/
 */
#include "graphics.h"

void drawPixel(int x, int y, int color) {
    Uint32 *pixel;

    pixel = (Uint32*) screen->pixels + x + y; // Magic pointer arithmetic; pixels are stored in linear array
    *pixel = color;
}
void drawFilledRect(int xi, int yi, int len, int width, int r, int g, int b) {
    int color, ytimesw;
    color = SDL_MapRGB(screen->format, r, g, b);
    int x, y;
    for (y=0; y < len; y++) {
        ytimesw = (yi+y)*screen->pitch/4;
        for (x=0; x < width; x++) {
            drawPixel(xi+x, ytimesw, color);
        }
    }
}

/*
 * TODO: Make this work for vertical and diagonal lines.  Currently only works for horizontal.
 */
void drawLine(int xi, int yi, int xf, int yf, int r, int g, int b) {
    int color;
    color = SDL_MapRGB(screen->format, r, g, b);

    int x, ytimesw;
    ytimesw = yi*screen->pitch/4;
    for( x = 0; x < (xf-xi); x++ )
    {
        drawPixel(xi+x, ytimesw, color);
    }

}

void updateScreen() {

    if(SDL_MUSTLOCK(screen)) {
        if(SDL_LockSurface(screen) < 0) return; //Lock surface for directly accessing pixels
    }


    /* If there has been a change to the drawing, draw it */
    if (mouse.xcor >= 0) {
        drawFilledRect(mouse.xcor, mouse.ycor, 5, 5, 0, 0, 255);


    }

    if(SDL_MUSTLOCK(screen)) SDL_UnlockSurface(screen); //Unlocks surface, done writing
    SDL_Flip(screen); //Swap image buffers
}
