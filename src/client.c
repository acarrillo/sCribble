#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "client.h"
#define AM_SERVER 1
#include "defs.h"


/*
 * Called once in init.c
 */
void initClient(char *addr) {
    //make the server socket for reliable IPv4 traffic
    socket_id = socket( AF_INET, SOCK_STREAM, 0);

    printf("Socket file descriptor: %d\n", socket_id);

    //set up the server socket struct, use IPv4
    sock.sin_family = AF_INET;

    //Client will connect to address in serverIP, need to translate that IP address to binary
    inet_aton( addr, &(sock.sin_addr) );

    //set the port to listen on, htons converts the port number to network format
    sock.sin_port = htons(44444);

    //connect to the server
    int c = connect(socket_id, (struct sockaddr *)&sock, sizeof(sock));
    printf("Connect returned: %d\n", c);

    //register with the current document using a CONNECT message
    buffer.type = C_CONNECT;
    b = write(socket_id, &buffer, sizeof(buffer) + 1);
}

void run_client(){
    buffer.type = C_PEN;

    printf("Enter message: ");
    fgets(buffer.data, sizeof(buffer.data), stdin);
    *(strchr(buffer.data, '\n')) = 0;

    b = write( socket_id, &buffer, sizeof(buffer) + 1 );

    if ( strncmp(buffer.data, "exit", sizeof(buffer)) == 0) {
        break;
    }

    b = read( socket_id, &buffer, sizeof(buffer));

    printf("\tReceived: %s\n", buffer.data);

    return 1;
}

void cleanup_client() {
    //unregister with the current document using a DISCONNECT message
    buffer.type = C_DISCONNECT;
    b = write(socket_id, &buffer, sizeof(buffer) +1);

    close(socket_id);
}
