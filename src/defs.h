#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "SDL/SDL.h"

typedef struct Mouse {
    int xcor;
    int ycor;
    int lastx;
    int lasty;
} Mouse;

struct ToolAction {
  int timestamp;
  char type[64];
  int color[3];
  int size;
  char data[128];
};
