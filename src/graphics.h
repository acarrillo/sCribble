#include "defs.h"
#define BORDER_WIDTH 6
#define C_SQUARE     25

#define BLACK        0
#define RED          1
#define ORANGE       2
#define YELLOW       3
#define GREEN        4
#define BLUE         5
#define INDIGO       6
#define VIOLET       7
#define WHITE        8

extern SDL_Surface *screen;
extern Mouse mouse;
extern int tool_width;
extern Color color;
extern TTF_Font *font(char*, int);

char gtag[256] = "GRAPHICS.C -- ";
