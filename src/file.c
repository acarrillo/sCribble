/**
 * file.c
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

#include "file.h"


/*
 * Attempts to save a .bmp of the canvas
 */
void saveImage() {
    SDL_Surface *screen_to_save;
    screen_to_save = SDL_DisplayFormat(screen);
    SDL_Rect bounds;

    bounds.x = BORDER_WIDTH;
    bounds.y = BORDER_WIDTH;
    bounds.w = 100;
    bounds.h = 100;
    /*bounds.w = screen->w - BORDER_WIDTH;*/
    /*bounds.h = screen->h - BORDER_WIDTH - (screen->h - 446); // See line 226 of graphics.c for magic number #FIXME*/

    SDL_BlitSurface(screen,  &bounds, screen_to_save, &bounds);

    if (SDL_SaveBMP(screen_to_save, "mypicture.bmp") < 0) {
        printf("Could not save canvas\n");
    }
    else {
        printf("Canvas saved successfully to 'mypicture.bmp'\n");
    }
    SDL_FreeSurface(screen_to_save);
}
