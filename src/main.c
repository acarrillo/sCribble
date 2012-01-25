/*
 * SDL framework borrowed from http://www.parallelrealities.co.uk/
 */

#include "main.h"
#include "client.h"

extern void init(char *);
extern void cleanup(void);
extern void updateScreen(void);
extern void getInput(void);

int main(int argc, char *argv[]) {
    
    //I can't figure this out. 
    /*  if(argc == 2){
    if(inet_pton(AF_INET, argv[1], NULL))
      initClient(argv[1]); //can't access initClient function from here.
    else
      printf("Error occurred in second parameter. \n");
  }
  //else 
    //execvp("the server program");?
    //send loopback address to client?
   */

    init("sCribble"); //Starts SDL


    atexit(cleanup); //Tells SDL to call the cleanup() function in init.c when exiting


    while (1) {
        getInput();

        updateScreen();

        SDL_Delay(16);
    }

    exit(0);
}
