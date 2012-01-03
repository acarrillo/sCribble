/*
 * SDL framework borrowed from http://www.parallelrealities.co.uk/
 */
#include "input.h"

void getInput()
{
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
                        break;
                }
                break;
            case SDL_MOUSEBUTTONDOWN:
                printf("%sbutton down caught\n", tag);
                switch (event.button.button) {
                    case SDL_BUTTON_LEFT:
                        if (event.button.state == SDL_PRESSED) {
                            mouse.xcor = event.button.x;
                            mouse.ycor = event.button.y;
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
                break;
            case SDL_MOUSEMOTION:
                if (SDL_GetMouseState(NULL, NULL)&SDL_BUTTON(1)) { //If mouse is down
                    mouse.xcor = event.button.x;
                    mouse.ycor = event.button.y;
                    
                    //added to deter segfaults in the vertical boundaries of the screen
                     if(mouse.ycor > 475 || mouse.ycor <= 2){
		      mouse.xcor = -1;
		      mouse.ycor = -1;
		    }
                    /*printf("%sxcor:%d\tycor:%d\n", tag, event.button.x, event.button.y);*/
                }
                break;
        }
    }
}
