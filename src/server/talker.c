#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <unistd.h>
#include "server.h"

void blastMessage(char* message){
  cribblePacket buffer;
  buffer.type = C_STATUS;
  strcpy(buffer.data, message);
  int i;
  for(i=0; i<64; i++)
    write( clientList[i], &buffer, sizeof(buffer));
}

void removeClient(int sock_client){
  //Remove client from the clients list
  int i;
  for(i=0;i<64; i++)
    if(clientList[i] == sock_client)
      clientList[i] = 0;
  close(sock_client);
  //Tell everyone about it
    char message[140];
    strcpy(message, messagePot->data);
    strcat(message, " left the doc");
    blastMessage(message);
}

void server_talker(int clientListKey){
  struct sembuf sop;
  sop.sem_num = 0;
  sop.sem_flg = 0;

  while(1){
    //Block until the pot semaphore is ready
    sop.sem_op = 0;
    semop(semid, &sop, 1);
    if(messagePot->type == C_DISCONNECT)
      removeClient(atoi(messagePot->data)); //Disconnect messages already have the client descriptor inserted into their message
    else {
      int i;
      //   strcat(data.data,"SERVER");
      for(i=0; i<64; i++)
	write( clientList[i], &messagePot, sizeof(messagePot));
    }
    sop.sem_op = 3;
    semop(semid, &sop, 1);
  }
}
