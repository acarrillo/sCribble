#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "SDL/SDL.h"
#include "SDL/SDL_ttf.h"

#define READ 0
#define WRITE 1

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

struct cribblePacket {
  char type[64];  //CONNECT, DISCONNECT, STATUS, PEN, LINE
  Color color;
  int size;
  char data[128];
};
