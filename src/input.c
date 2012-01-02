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
                switch (event.button.button) {
                    case SDL_BUTTON_LEFT:
                        if (event.button.state == SDL_PRESSED) {
                            printf("xcor:%d\tycor:%d\n", event.button.x, event.button.y);
                        }
                        break;
                    default:
                        break;
                }
                break;
        }
    }
}
