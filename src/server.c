#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
//#include "defs.h"

struct cribblePacket {
  char type[64];
  int color[3];
  int size;
  char data[128];
};

void addClient(int sock_client, int* clients){
  int i;
  for(i=0;i<64; i++)
    if(!clients[i]){
      clients[i] = sock_client;
      break;
    }
}

void removeClient(int sock_client, int* clients){
  int i;
  for(i=0;i<64; i++)
    if(clients[i] == sock_client)
      clients[i] = 0;
}

blastMessage(char* message, int* clients){
  struct cribblePacket buffer;
  strcpy(buffer.type, "STATUS");
  strcpy(buffer.data, message);
  int i;
  for(i=0; i<64; i++)
    write( clients[i], buffer, sizeof(buffer));
}

void serve(struct cribblePacket data, int sock_client, int* clients){
  if(strcmp(data.type, "CONNECT")){
    char message[142];
    strcpy(message, data.data);
    strcat(message, " joined the doc");
    blastMessage(message, clients);
    addClient(sock_client, clients);
  }
  else if(strcmp(data.type, "DISCONNECT")){
    char message[140];
    strcpy(message, data.data);
    strcat(message, " left the doc");
    blastMessage(message, clients);
    removeClient(sock_client, clients);
  }
  else{ //It must be a drawing tool
    int i;
    for(i=0; i<64; i++)
      write( clients[i], data, sizeof(data));
  }
}

int main() {

  int socket_id, socket_client;
  struct cribblePacket buffer;
  int i, b;
  struct sockaddr_in server;
  socklen_t socket_length;

  int clients[64];

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
  i =  listen( socket_id, 0 );

  //acept connections continuously
  while(1) {

    printf("Waiting for a connection\n");

    //set socket_length after the connection is made
    socket_length = sizeof(server); 
    int l = sizeof(server); 

    //accept the incoming connection, create a new file desciprtor for the socket to the client | BLOCKS
    socket_client = accept(socket_id, (struct sockaddr *)&server, &l);

    printf("accepted connection %d\n",socket_client);

    b = read( socket_client, buffer, sizeof(buffer) );
    serve(buffer, socket_client, clients);
      
    //close this client connection
    close(socket_client);
  }
}
