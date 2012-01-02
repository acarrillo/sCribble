/*
 * SDL framework borrowed from http://www.parallelrealities.co.uk/
 */

#include "main.h"

extern void init(char *);
extern void cleanup(void);
extern void updateScreen(void);
extern void getInput(void);

void do_canvas() {
    updateScreen();
    SDL_Delay(16);
}

void do_interactions() {
    getInput();
    SDL_Delay(4);
}

int main(int argc, char *argv[]) {

    init("sCribble"); //Starts SDL


    atexit(cleanup); //Tells SDL to call the cleanup() function in init.c when exiting

    assert( pipe(toCanvas) == 0 ); // Pipe for the interactions controller to write to the canvas
    assert( pipe(toController) == 0 ); // Pipe for the canvas to write to the interactions controller

    int p;
    p = fork();

    while (1) {
        if (!p) {
            do_interactions();
        }
        else {
            do_canvas();
        }
    }
    exit(0);
}
