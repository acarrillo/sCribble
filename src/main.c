/**
 * main.c
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

    // Defaults it to localhost
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
