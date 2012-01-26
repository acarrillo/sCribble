/*
 * SDL framework borrowed from http://www.parallelrealities.co.uk/
 */
#include <arpa/inet.h>
#include <unistd.h>
#include "main.h"

extern void init(char *, char *);
extern void cleanup(void);
extern void updateScreen(void);
extern void getInput(void);

extern void run_client(); // From client.c
extern void initClient(char* addr);

int main(int argc, char *argv[]) {



  if(argc == 2){
    if(inet_pton(AF_INET, argv[1], NULL)){
      initClient(argv[1]);
    }
    else
      printf("Error occurred in second parameter. \n");
  }
  else if(argc == 1)
    execlp("server", "server", NULL);
  //send loopback address to client? how do?



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
