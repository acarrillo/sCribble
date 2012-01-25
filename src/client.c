#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "client.h"
#include <errno.h>
#include <fcntl.h>

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
    // printf("Connect returned: %d\n", c);
    if (c == -1) printf("connect returned %d with an error \"%s\"\n", c, strerror(errno));
    
    //mark client socket as non-blocking
    fcntl(socket_id, F_SETFL, O_NONBLOCK);

    //register with the current document using a CONNECT message
    cPacket.type = C_CONNECT;
    b = write(socket_id, &cPacket, sizeof(cPacket));
    printf("Send a connect message of %d bytes\n", b);
}

/*
 * Called iteratively in main.c
 */
void run_client(){

    b = write( socket_id, &cPacket, sizeof(cPacket));
    if (b == -1) printf("write returned %d with an error \"%s\"\n", b, strerror(errno));
    printf("\tSent %d bytes\n", b);
    b = read( socket_id, &cPacket, sizeof(cPacket));
    if (b == -1) printf("read returned %d with an error \"%s\"\n", b, strerror(errno));
    printf("\tReceived %d bytes\n", b);
}

void cleanup_client() {
    //unregister with the current document using a DISCONNECT message
    cPacket.type = C_DISCONNECT;
    b = write(socket_id, &cPacket, sizeof(cPacket) +1);

    close(socket_id);
}
