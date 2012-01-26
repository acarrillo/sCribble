/**
 * defs.h
 * Copyright (c) 2012 Alejandro Carrillo, Doron Shapiro, and Vincent Zhen
 *
 * This file is part of sCribble.
 *
 * sCribble is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * sCribble is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with sCribble.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#ifndef AM_SERVER
  #include "SDL/SDL.h"
  #include "SDL/SDL_ttf.h"
  #include "SDL/SDL_image.h"
#endif

#define READ 0
#define WRITE 1

#define C_CONNECT 0
#define C_DISCONNECT 1
#define C_STATUS 2
#define C_PEN 3
#define C_LINE 4

#define BORDER_WIDTH 6
#define BORDER_COLOR 50
#define C_SQUARE     25

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
