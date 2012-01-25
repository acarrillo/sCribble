#include <stdio.h>
#include <unistd.h>
#include <sys/sem.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include "server.h"

void subserve(int socket_client){
  int b, done;
  cribblePacket buffer;
  struct sembuf sop;
  sop.sem_num = 0;
  sop.sem_flg = 0;
  done = 0;

  printf("subserver init'd\n");

 //mark subserver socket as blocking
  int opts = fcntl(socket_client,F_GETFL);
  fcntl(socket_client, F_SETFL, opts & (~O_NONBLOCK));

  while(!done){
    //read from the client (blocks)
    b = read( socket_client, &buffer, sizeof(buffer) );
    if(buffer.type != C_CONNECT){
      printf("Subserver: got %d message\n", buffer.type);      

      //grab the semaphore
      sop.sem_op = -2;
      semop(semid, &sop, 1);
      
      if(buffer.type == C_DISCONNECT){
	sprintf(messagePot->data, "%d", socket_client);
	done = 1;
      }
      
      memcpy(messagePot, &buffer, sizeof(buffer));
      
      //release the semaphore to the main server
      printf("\tSubserver: releasing the semaphore\n");
      sop.sem_op = -1;
      semop(semid, &sop, 1);
    }
  }
}
