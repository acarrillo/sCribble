/**
 * server.c
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
#include <stdio.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <stdint.h>
#include <sys/sem.h>
#include <unistd.h>
#include <signal.h>
#include "server.h"
#include "listener.h"
#include "semun.h"
#include <errno.h>
#include <signal.h>

/* FROM server.h :
  int semid;
  cribblePacket* messagePot;
  int* clientList;
*/

int clientListDescriptor, messagePotDescriptor;

/*********************************************************************
 * Unlink and remove shared memory
 * Remove semaphore
 *********************************************************************/
void cleanup(){
  //unlink and remove shared memory
  printf("\t(cleaning up...)\n");
  kill(0,2);
  shmdt(messagePot);
  shmctl(messagePotDescriptor, IPC_RMID, NULL);
  semctl(semid, 0, IPC_RMID);
}

static void sighandler(int signo) {
  printf("Caught %d\n", signo);
  cleanup();
  if ( signo == SIGINT )
    exit(-1);
}

int main(int argc, char *argv[]){
  union semun semop;

  // signal(SIGSEGV, sighandler);
  signal(SIGINT, sighandler);

  //Set up the messagePot in shared memory
  messagePotDescriptor = shmget(MESSAGE_POT_KEY, 64*sizeof(int), 0664 | IPC_CREAT);
  messagePot = (cribblePacket*) shmat(messagePotDescriptor, NULL, 0);
  /*printf("messagePotDescriptor: %d\n",messagePotDescriptor);*/

  //Set up a semaphore for the messagePot
  semid = semget(MESSAGE_POT_SEM, 1, 0664 | IPC_CREAT);
  semop.val = POT_EMPTY;
  semctl(semid, 0, SETVAL, semop);

  //accept incoming network connections
  loop_server();
  cleanup();
  return 1;
}
