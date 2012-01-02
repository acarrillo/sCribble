/*
 * SDL framework borrowed from http://www.parallelrealities.co.uk/
 */
#include "graphics.h"

void drawFilledRect(int x, int y, int len, int width, int r, int g, int b) {
    int color;
    color = SDL_MapRGB(screen->format, r, g, b);
    int i, j;
    for (i; i < len; i++) {
        for (j; j < width; j++) {
            drawPixel(i+x, j+y, color);
        }
    }
}
void drawPixel(int x, int y, int color) {

    int *pixel;

    pixel = screen->pixels + (640*y) + (x*4); // Magic pointer arithmetic, dwai.
    *pixel = color;
}

void updateScreen() {
    /* Blank the screen */

    SDL_FillRect(screen, NULL, 255);

    drawPixel(100, 100, 255, 0, 0);
    SDL_Flip(screen); //Swap image buffers
}
