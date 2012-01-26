/**
 * input.c
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
#include "input.h"

/*extern cribblePacket cribblePacket;*/

void getInput()
{
    Uint8 *keystate; // Used for getting cmd-q
	SDL_Event event;
	/* Loop through waiting messages and process them */
	while (SDL_PollEvent(&event)) {
        switch (event.type) {
            /* Closing the Window or pressing Escape will exit the program */

            case SDL_QUIT:
                exit(0);
                break;

            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                    case SDLK_ESCAPE:
                        exit(0);
                        break;
                    default:
                        keystate = SDL_GetKeyState(NULL);
                        if ((keystate[SDLK_RMETA] || keystate[SDLK_LMETA]) && keystate[SDLK_q]) { // Apple cmd-Q
                            exit(0);
                        }
                        break;
                }
                break;
            case SDL_MOUSEBUTTONDOWN:
                /*printf("%sbutton down caught\n", tag);*/
                switch (event.button.button) {
                    case SDL_BUTTON_LEFT:
                        if (event.button.state == SDL_PRESSED) {
                            mouse.xcor = event.button.x;
                            mouse.ycor = event.button.y;
                            
                            //cribblePacket stuff
                            /*
                            (cribblePacket.mouse).xcor = event.button.x;
                            (cribblePacket.mouse).ycor = event.button.y;
                            */
                            
                            /*printf("%sxcor:%d\tycor:%d\n", tag, event.button.x, event.button.y);*/
                        }
                        break;
                    default:
                        break;
                }
                break;
            case SDL_MOUSEBUTTONUP:
                /* Set the mice coords to -1 to notify others of no change */
                mouse.xcor = -1;
                mouse.ycor = -1;
                mouse.lastx = -1;
                mouse.lasty = -1;
                
                //cribblePacket stuff
                /*
                (cribblePacket.mouse).lastx = -1;
                (cribblePacket.mouse).lasty = -1;
                (cribblePacket.mouse).xcor = -1;
                (cribblePacket.mouse).ycor = -1;
        	*/
        	
                break;
            case SDL_MOUSEMOTION:
                if (SDL_GetMouseState(NULL, NULL)&SDL_BUTTON(1)) { //If mouse is down
                    mouse.xcor = event.button.x;
                    mouse.ycor = event.button.y;
                    
                    //cribblePacket stuff
                    /*
                    (cribblePacket.mouse).xcor = event.button.x;
                    (cribblePacket.mouse).ycor = event.button.y;
                    */

                    //added to deter segfaults in the vertical boundaries of the screen
                    if(mouse.ycor > 425 || mouse.ycor <= 2){
                        mouse.xcor = -1;
                        mouse.ycor = -1;
                        
                        //cribblePacket stuff
                        /*
                        (cribblePacket.mouse).xcor = -1;
                        (cribblePacket.mouse).ycor = -1;
                    	*/
                    	
                    }
                    /*printf("%sxcor:%d\tycor:%d\n", tag, event.button.x, event.button.y);*/
                }
                break;
        }
    }
}
