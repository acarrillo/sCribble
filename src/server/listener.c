#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include "server.h"
#include "subserver.h"
#include <errno.h>

int socket_id;
struct sockaddr_in server;

void server_listener_setup(){
  int i;

  //make the server socket for reliable IPv4 traffic
  socket_id = socket( AF_INET, SOCK_STREAM, 0);
  printf("Socket file descriptor: %d\n", socket_id);

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

int add_client(int sock_client){
  int i;
  for(i=0;i<64; i++){
    printf("checking client #%d\n",i);
    if(!clientList[i]){
      clientList[i] = sock_client;
      break;
    }
  }
  return 64 - i;
}

void server_listener(){
  int socket_client, b, subserver;
  struct cribblePacket buffer;
  socklen_t socket_length;

  server_listener_setup();
  while(1) {
    printf("Listener: Waiting for a connection\n");

    //set socket_length after the connection is made
    socket_length = sizeof(server);
    socklen_t l = sizeof(server);

    //accept the incoming connection, create a new file desciptor for the socket to the client | BLOCKS
    socket_client = accept(socket_id, (struct sockaddr *)&server, &l);
    //printf( "Error: %s\n", strerror( errno ) );

    printf("Listener: accepted connection %d\n",socket_client);

    b = read( socket_client, &buffer, sizeof(buffer) );
    if(buffer.type == C_CONNECT){
      printf("Listener: got connect message\n");
      add_client(socket_client);
      subserver = fork();
      if(subserver == 0)
      	subserve(socket_client);
    }
  }
}
