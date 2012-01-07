#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "SDL/SDL.h"
#include "SDL/SDL_draw.h"

typedef struct Mouse {
    int xcor;
    int ycor;
    int lastx;
    int lasty;
} Mouse;

typedef struct Color {
    Uint8 r; // 0 to 255
    Uint8 g;
    Uint8 b;
} Color;
