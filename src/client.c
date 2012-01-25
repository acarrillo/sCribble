#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "client.h"


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
    cPacket.type = C_CONNECT;
    b = write(socket_id, &cPacket, sizeof(cPacket) + 1);
}

/*
 * Called iteratively in main.c
 */
void run_client(){

    b = write( socket_id, &cPacket, sizeof(cPacket) + 1 );

    b = read( socket_id, &cPacket, sizeof(cPacket));

    printf("\tReceived: %s\n", cPacket.data);
}

void cleanup_client() {
    //unregister with the current document using a DISCONNECT message
    cPacket.type = C_DISCONNECT;
    b = write(socket_id, &cPacket, sizeof(cPacket) +1);

    close(socket_id);
}
