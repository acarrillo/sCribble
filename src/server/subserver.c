#include <stdio.h>
#include <unistd.h>
#include <sys/sem.h>
#include <string.h>
#include "server.h"

void subserve(int socket_client){
  int b;
  cribblePacket buffer;
  struct sembuf sop;
  sop.sem_num = 0;
  sop.sem_flg = 0;
  
  while(1){
    //read from the client (blocks)
    b = read( socket_client, &buffer, sizeof(buffer) );
    
    //grab the semaphore
    sop.sem_op = -2;
    semop(semid, &sop, 1);
   
    memcpy(messagePot, &buffer, sizeof(buffer));
    
    if(messagePot->type == C_DISCONNECT)
      break;

    //release the semaphore to the main server
    sop.sem_op = -1;
    semop(semid, &sop, 1);
  }
}
