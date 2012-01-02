/*
 * SDL framework borrowed from http://www.parallelrealities.co.uk/
 */

#include "main.h"

extern void init(char *);
extern void cleanup(void);
extern void updateScreen(void);
extern void getInput(void);

int main(int argc, char *argv[]) {

    init("sCribble"); //Starts SDL


    atexit(cleanup); //Tells SDL to call the cleanup() function in init.c when exiting


    while (1) {
        getInput();

        updateScreen();

        SDL_Delay(16);
    }

    exit(0);
}
