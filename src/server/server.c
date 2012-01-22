#include <stdio.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <stdint.h>
#include <sys/sem.h>
#include <unistd.h>
#include <signal.h>
#define AM_SERVER 1
#include "talker.h"
#include "server.h"
#include "listener.h"
#include <errno.h>

/* FROM server.h : 
  int semid;
  cribblePacket* messagePot;
  int* clientList;
*/

int clientListDescriptor, messagePotDescriptor;

void cleanup(){
  //unlink and remove shared memory
  printf("\t(cleaning up...)\n");
  shmdt(clientList);
  shmctl(clientListDescriptor, IPC_RMID, NULL);
  shmdt(messagePot);
  shmctl(messagePotDescriptor, IPC_RMID, NULL);
}

static void sighandler(int signo) {
  printf("Caught %d\n", signo);
  cleanup();  //So we don't leave junk memory behind
  if ( signo == SIGINT )
    exit(-1);
}

int main(int argc, char *argv[]){
  int talker;
  union semun semop;
  
  // signal(SIGSEGV, sighandler);
  signal(SIGINT, sighandler);

  //Set up a list of client socket descriptors in shared memory
  clientListDescriptor = shmget(CLIENT_LIST_KEY, 64*sizeof(int), 0666 | IPC_CREAT);
  clientList = (int*) shmat(clientListDescriptor, NULL, 0);
  printf("clientListDescriptor: %d\n",clientListDescriptor);

  //Set up the messagePot in shared memory
  messagePotDescriptor = shmget(MESSAGE_POT_KEY, 64*sizeof(int), 0664 | IPC_CREAT);
  messagePot = (cribblePacket*) shmat(messagePotDescriptor, NULL, 0);  
  printf("messagePotDescriptor: %d\n",messagePotDescriptor);

  //Set up a semaphore for the messagePot
  semid = semget(MESSAGE_POT_SEM, 1, 0664 | IPC_CREAT | IPC_EXCL);
  semop.val = POT_EMPTY;
  semctl(semid, 0, SETVAL, semop);

  talker = fork();
   if(talker == 0)
     server_talker(CLIENT_LIST_KEY); //broadcasts outgoing network connections
   else
     server_listener();  //accepts incoming network connections
   
   cleanup();
   return 1;
}
