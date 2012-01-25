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

#include "talker.h"

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

void server_listener(){
  int socket_client, b, subserver;
  struct cribblePacket buffer;
 /*****************************************FROM TALKER*****************************************************************************/
  //int b;
  int i;
  struct sembuf sop;
  sop.sem_num = 0;
  sop.sem_flg = IPC_NOWAIT; //Do not block
 /*********************************************************************************************************************************/

  server_listener_setup();
  while(1) {
    // printf("Listener: Waiting for a connection\n");

    //set socket_length after the connection is made
    socklen_t l = sizeof(server);

    //accept the incoming connection, create a new file desciptor for the socket to the client
    socket_client = accept(socket_id, (struct sockaddr *)&server, &l);
    if(socket_client != -1){
      do {b = read( socket_client, &buffer, sizeof(buffer) );} while (b==-1);
      if (b == -1) printf("read returned %d with an error \"%s\"\n", b, strerror(errno));
      printf("Listener accepted, got a %d message saying socket client is %d\n", buffer.type, socket_client);
      if(buffer.type == C_CONNECT){
	printf("Listener: got connect message\n");
	add_client(socket_client);
	subserver = fork();
	if(subserver == 0)
	  subserve(socket_client);
      }
    }
    else if(errno != EAGAIN) printf( "accept error in listener: %s\n", strerror( errno ) );
    
    /*****************************************FROM TALKER*****************************************************************************/
    sop.sem_op = 0;
    i = semop(semid, &sop, 1);
    
    if(i != -1) { //The semaphore is ready!
      printf("Talker: semid is %d\n", semid);
      printf("Talker: Semaphore is ready (i = %d)\n", i);
      
      if(messagePot->type == C_DISCONNECT){
	printf("Talker: disconnecting client\n");
	removeClient(atoi(messagePot->data)); //Disconnect messages already have the client descriptor inserted into their message
      }
      else {
	printf("Talker: broadcasting...\n");
	if(messagePot->type == C_PEN) broadcastPacket(messagePot);
      }

      printf("Talker: releasing semaphore\n");
      sop.sem_op = 3;
      semop(semid, &sop, 1);
    }
    else if(errno != EAGAIN) printf( "Semop error in talker: %s\n", strerror( errno ) );
    /*********************************************************************************************************************************/
    
  }
}
