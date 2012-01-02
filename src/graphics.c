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
    int x, y, ytimesw;
    color = SDL_MapRGB(screen->format, r, g, b);

    float slope;
    slope = ((float)yf-yi)/((float)xf-xi);
    printf("%sslope:%2.2f\n",gtag, slope);
    for ( y = 0; y < (yf-yi); y++) {
        ytimesw = (y+yi)*screen->pitch/4;
        for( x = 0; x < (xf-xi); x++ ) {
            if (slope*x == y) {
                drawPixel(xi+x, ytimesw, color);
            }
        }
    }
}

void updateScreen() {

    if(SDL_MUSTLOCK(screen)) {
        if(SDL_LockSurface(screen) < 0) return; //Lock surface for directly accessing pixels
    }


    /* If there has been a change to the drawing, draw it */
    if (mouse.xcor >= 0) {
        /* TODO: Scale up from simple pen tool */
        drawFilledRect(mouse.xcor-2, mouse.ycor-2, 5, 5, 0, 255, 255);
    }

    drawLine(200, 100, 400, 200, 255, 0, 0);

    if(SDL_MUSTLOCK(screen)) SDL_UnlockSurface(screen); //Unlocks surface, done writing
    SDL_Flip(screen); //Swap image buffers
}
