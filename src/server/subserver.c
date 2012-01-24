#include <stdio.h>
#include <unistd.h>
#include <sys/sem.h>
#include <string.h>
#include <stdlib.h>
#include "server.h"

void subserve(int socket_client){
  int b, done;
  cribblePacket buffer;
  struct sembuf sop;
  sop.sem_num = 0;
  sop.sem_flg = 0;
  done = 0;

  while(!done){
    //read from the client (blocks)
    b = read( socket_client, &buffer, sizeof(buffer) );
    
    //grab the semaphore
    sop.sem_op = -2;
    semop(semid, &sop, 1);
   
    if(messagePot->type == C_DISCONNECT){
      sprintf(messagePot->data, "%d", socket_client);
      done = 1;
    }

    //change the message (for testing only)
    strcat(buffer.data,"SRV");

    memcpy(messagePot, &buffer, sizeof(buffer));

    //release the semaphore to the main server
    sop.sem_op = -1;
    semop(semid, &sop, 1);
  }
}
