/**
 * listener.c
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
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#include "server.h"
#include "subserver.h"
#include <errno.h>

#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <unistd.h>

int socket_id;
struct sockaddr_in server;

void server_listener_setup(){
  int i;

  //make the server socket for reliable IPv4 traffic
  socket_id = socket( AF_INET, SOCK_STREAM, 0);
  printf("Socket file descriptor: %d\n", socket_id);

  //mark server socket as non-blocking
  fcntl(socket_id, F_SETFL, O_NONBLOCK);

  //set up the server socket struct (use IPv4)
  server.sin_family = AF_INET;

  //This is the server, so it will listen to anything coming into the host computer
  server.sin_addr.s_addr = INADDR_ANY;
  
  //set the port to listen on, htons converts the port number to network format
  server.sin_port = htons(44444);
  
  //bind the socket to the socket struct
  i = bind( socket_id, (struct sockaddr *)&server, sizeof(server) );

  //wait for any connection
  i = listen( socket_id, 0 );    
}

void broadcastPacket(cribblePacket* packet){
  // printf("Message to broadcast: %s.\n", packet->data);
  int i, b;
  for(i=0; i<64; i++){
    if(clientList[i]){
      b = write( clientList[i], packet, sizeof(*packet));
      if(b==-1) printf( "Talker writing error: %s\n", strerror( errno ) );
      printf("\tTalker wrote %d bytes to %d\n", b, clientList[i]);
    }
  }
}

void broadcastMessage(char* message){
  cribblePacket buffer;
  buffer.type = C_STATUS;
  strcpy(buffer.data, message);
  broadcastPacket(&buffer);
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
    broadcastMessage(message);
}

int add_client(int sock_client){
  int i;
  for(i=0;i<64; i++){
    if(!clientList[i]){
      clientList[i] = sock_client;
      break;
    }
  }
  printf("Listener added client: %d\n", sock_client);
  return 64 - i;
}

void server_listen(){
  int socket_client, b, subserver;
  struct cribblePacket buffer;

  //set socket_length after the connection is made
  socklen_t l = sizeof(server);

  //accept the incoming connection, create a new file desciptor for the socket to the client | N.B. does not block
  socket_client = accept(socket_id, (struct sockaddr *)&server, &l);
  if(socket_client != -1){ //If there was a message for us...
    do {b = read( socket_client, &buffer, sizeof(buffer) );} while (b==-1); //Read it!
    if (b == -1) printf("read returned %d with an error \"%s\"\n", b, strerror(errno));

    if(buffer.type == C_CONNECT){
      printf("Listener: got connect message\n");
      add_client(socket_client);
      subserver = fork();
      if(subserver == 0)
	subserve(socket_client);
    }
  }
  else if(errno != EAGAIN) printf( "accept error in listener: %s\n", strerror( errno ) );
}

void server_talk(){
  int i;
  struct sembuf sop;
  sop.sem_num = 0;
  sop.sem_flg = IPC_NOWAIT; //Do not block

  sop.sem_op = 0;
  i = semop(semid, &sop, 1);
    
  if(i != -1) { //The semaphore is ready!
    printf("Talker: Semaphore is ready (i = %d)\n", i);
      
    if(messagePot->type == C_DISCONNECT){
      printf("Talker: disconnecting client\n");
      removeClient(atoi(messagePot->data)); //Disconnect messages already have the client descriptor inserted into their message
    }
    else {
      printf("\tTalker: broadcasting...\n");
      if(messagePot->type != C_CONNECT) broadcastPacket(messagePot);
    }

    printf("\tTalker: releasing semaphore\n");
    sop.sem_op = 3;
    semop(semid, &sop, 1);
  }
  else if(errno != EAGAIN) printf( "Semop error in talker: %s\n", strerror( errno ) );
}

void loop_server(){
  server_listener_setup();
  while(1) {
    server_talk();
    server_listen();
  }
}
