#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "client.h"
#include "defs.h"

void run_client(char* serverIP){

  int socket_id;
  struct cribblePacket buffer;
  int b;
  struct sockaddr_in sock;

  //make the server socket for reliable IPv4 traffic 
  socket_id = socket( AF_INET, SOCK_STREAM, 0);

  printf("Socket file descriptor: %d\n", socket_id);

  //set up the server socket struct, use IPv4 
  sock.sin_family = AF_INET;

  //Client will connect to address in serverIP, need to translate that IP address to binary
  inet_aton( serverIP, &(sock.sin_addr) );
    
  //set the port to listen on, htons converts the port number to network format
  sock.sin_port = htons(24601);
  
  //connect to the server
  int c = connect(socket_id, (struct sockaddr *)&sock, sizeof(sock));
  printf("Connect returned: %d\n", c);

  //register with the current document using a CONNECT message
  strcpy(buffer.type, "CONNECT");
  b = write(socket_id, &buffer, sizeof(buffer) + 1);

    //do client stuff continuously
    while (1) {
      strcpy(buffer.type, "PEN");

      printf("Enter message: ");
      fgets(buffer.data, sizeof(buffer.data), stdin);
      *(strchr(buffer.data, '\n')) = 0;

      b = write( socket_id, &buffer, sizeof(buffer) + 1 );

      if ( strncmp(buffer.data, "exit", sizeof(buffer)) == 0)
	break;

      b = read( socket_id, &buffer, sizeof(buffer));
      
      printf("\tReceived: %s\n", buffer.data);
    }

    //unregister with the current document using a DISCONNECT message
    strcpy(buffer.type, "DISCONNECT");
    b = write(socket_id, &buffer, sizeof(buffer) +1);

    close(socket_id);
}
