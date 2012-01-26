/*
 * SDL framework borrowed from http://www.parallelrealities.co.uk/
 */

#include "main.h"

extern void init(char *, char *);
extern void cleanup(void);
extern void updateScreen(void);
extern void getInput(void);

extern void run_client(); // From client.c

int main(int argc, char *argv[]) {

    init("sCribble", "127.0.0.1"); //Starts SDL, connects to 127.0.0.1


    atexit(cleanup); //Tells SDL to call the cleanup() function in init.c when exiting

    while (1) {
        getInput();

        run_client();

        updateScreen();

        SDL_Delay(16);
    }

    exit(0);
}
