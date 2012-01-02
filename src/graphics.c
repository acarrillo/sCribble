/*
 * SDL framework borrowed from http://www.parallelrealities.co.uk/
 */
#include "graphics.h"

void drawPixel(int x, int y, int color) {
    int *pixel;

    pixel = screen->pixels + x + y; // Magic pointer arithmetic; pixels are stored in linear array
    *pixel = color;
}
void setpixel(SDL_Surface *screen, int x, int y, Uint8 r, Uint8 g, Uint8 b)
{
    Uint32 *pixmem32;
    Uint32 colour;  
 
    colour = SDL_MapRGB( screen->format, r, g, b );
  
    pixmem32 = (Uint32*) screen->pixels  + y + x;
    *pixmem32 = colour;
}
/*void DrawScreen(SDL_Surface* screen)*/
void DrawScreen()
{ 
    int x, y, ytimesw;
    //Magic below
    for(y = 0; y < screen->h; y++ )
    {
        ytimesw = y*screen->pitch/4;
        for( x = 0; x < screen->w; x++ )
        {
            /*setpixel(screen, x, ytimesw, (x*x)/256+3*y+h, (y*y)/256+x+h, h);*/
            setpixel(screen, x, ytimesw, 255, 0, 0);
        }
    }
}
void drawFilledRect(int x, int y, int len, int width, int r, int g, int b) {
    int color;
    color = SDL_MapRGB(screen->format, r, g, b);
    int i, j;
    for (i=0; i < len; i++) {
        for (j=0; j < width; j++) {
            drawPixel(x, y, color);
        }
    }
}

void drawLine(int xi, int yi, int xf, int yf, int r, int g, int b) {
    int color;
    color = SDL_MapRGB(screen->format, r, g, b);

    int x, y, ytimesw;
    for(y = 0; y < screen->h; y++ )
    {
        ytimesw = y*screen->pitch/4;
        for( x = 0; x < screen->w; x++ )
        {
            drawPixel(x, ytimesw, color);
        }
    }

}

void updateScreen(int h) {

    if(SDL_MUSTLOCK(screen)) {
        if(SDL_LockSurface(screen) < 0) return; //Lock surface for directly accessing pixels
    }

    DrawScreen();

    /*drawLine(5, 5, 5, 5, 255, 0, 0);*/

    if(SDL_MUSTLOCK(screen)) SDL_UnlockSurface(screen); //Unlocks surface, done writing
    SDL_Flip(screen); //Swap image buffers
}
