/*
 * SDL framework borrowed from http://www.parallelrealities.co.uk/
 */

#include "main.h"

extern void init(char *);
extern void cleanup(void);
extern void updateScreen(int h);
extern void getInput(void);

int main(int argc, char *argv[]) {

    init("sCribble"); //Starts SDL


    atexit(cleanup); //Tells SDL to call the cleanup() function in init.c when exiting


    int h;
    h = 0;
    while (1) {
        getInput();

        updateScreen(h++);

        SDL_Delay(16);
    }

    exit(0);
}
