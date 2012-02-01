/**
 * graphics.h
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

/*
 * SDL framework borrowed from http://www.parallelrealities.co.uk/
 */
#include "defs.h"

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

char gtag[256] = "GRAPHICS.C -- ";
