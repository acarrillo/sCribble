/*
 * SDL framework borrowed from http://www.parallelrealities.co.uk/
 */
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include "main.h"

extern void init(char *, char *);
extern void cleanup(void);
extern void updateScreen(void);
extern void getInput(void);
extern void cleanup_client(void);

extern void run_client(); // From client.c
extern void initClient(char* addr);

int haveAServer;

int main(int argc, char *argv[]) {
  char serverIP[16];
  strncpy(serverIP, "127.0.0.1", 16);
  haveAServer = 0;


  if(argc == 2){ //we have been passed a server to connect to
    if(inet_addr(argv[1]))
      strncpy(serverIP, argv[1], 16);
    else {
      printf("Invalid IP Address. \n");
      exit(0);
    }
  }
  else if(argc == 1){ //we are hosting a server
    haveAServer = fork();
    if (haveAServer == 0){
      printf("starting server\n");
      execlp("./sCribbleServer", "./sCribbleServer", NULL);
      exit(0);
    }
    else{
      sleep(1);
    }
  }

  init("sCribble", serverIP); //Starts SDL, connects to 127.0.0.1


  atexit(cleanup); //Tells SDL to call the cleanup() function in init.c when exiting

  while (1) {
    getInput();

    run_client();

    updateScreen();

    SDL_Delay(16);
  }
  if(haveAServer){
    printf("killing server\n");
    kill(haveAServer, 2);
  }
  exit(0);
}
