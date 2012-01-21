#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "SDL/SDL.h"
#include "SDL/SDL_ttf.h"
#include "SDL/SDL_image.h"

#define READ 0
#define WRITE 1

#define C_CONNECT 0
#define C_DISCONNECT 1
#define C_STATUS 2
#define C_PEN 3
#define C_LINE 4

typedef struct Mouse {
    int xcor;
    int ycor;
    int lastx;
    int lasty;
} Mouse;

typedef struct Color {
    int id; // 0 to 8, constants defined in graphics.h
    uint8_t r; // 0 to 255
    uint8_t g;
    uint8_t b;
} Color;

typedef struct cribblePacket {
  int type;
  Color color;
  int tool_width;
  Mouse mouse;
  char data[128];
} cribblePacket;

typedef struct Circle {
  int savedx;
  int savedy;
  int iter;
} Circle;
