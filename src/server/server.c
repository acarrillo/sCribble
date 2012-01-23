#include <stdio.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <stdint.h>
#include <sys/sem.h>
#include <unistd.h>
#define AM_SERVER 1
#include "talker.h"
#include "server.h"
#include "listener.h"

/* FROM server.h : 
  int semid;
  cribblePacket* messagePot;
  int* clientList;
*/

int main(int argc, char *argv[]){
  int clientListDescriptor, talker, messagePotDescriptor;
  union semun semop;
  
  //Set up a list of client socket descriptors in shared memory
  clientListDescriptor = shmget(CLIENT_LIST_KEY, 64*sizeof(int), 0664 | IPC_CREAT | IPC_EXCL);
  clientList = (int*) shmat(clientListDescriptor, NULL, 0);
  
  //Set up the messagePot in shared memory
  messagePotDescriptor = shmget(MESSAGE_POT_KEY, 64*sizeof(int), 0664 | IPC_CREAT | IPC_EXCL);
  messagePot = (cribblePacket*) shmat(messagePotDescriptor, NULL, 0);  

  //Set up a semaphore for the messagePot
  semid = semget(MESSAGE_POT_SEM, 1, 0664 | IPC_CREAT | IPC_EXCL);
  semop.val = POT_EMPTY;
  semctl(semid, 0, SETVAL, semop);

  talker = fork();
   if(talker == 0)
     server_talker(CLIENT_LIST_KEY);
   else
  server_listener();
  
  //remove shared memory
  shmdt(clientList);
  shmdt(messagePot);

  return 1;
}
