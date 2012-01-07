#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "SDL/SDL.h"

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

struct ToolAction {
  int timestamp;
  char type[64];
  int color[3];
  int size;
  char data[128];
};
