/**
 * client.c
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
#include "client.h"

extern int local_tool_width; // From main.h

/*
 * Updates the cPacket buffer to reflect the changes made to it by the client.  Must be run AFTER canvas has drawn the server's cPacket.
 */
void update_cPacket() {
    cPacket.type = C_PEN;
    (cPacket.color).r=color.r;
    (cPacket.color).g=color.g;
    (cPacket.color).b=color.b;
    (cPacket.color).id=color.id;
    cPacket.tool_width = local_tool_width;
    (cPacket.mouse).xcor= mouse.xcor;
    (cPacket.mouse).ycor= mouse.ycor;
    (cPacket.mouse).lastx= mouse.lastx;
    (cPacket.mouse).lasty= mouse.lasty;
}

/*
 * Called once in init.c
 */
void initClient(char *addr) {
    //make the server socket for reliable IPv4 traffic
    socket_id = socket( AF_INET, SOCK_STREAM, 0);

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
}

/*
 * Clears buffer cPacket
 */
void clear_buffer() {
    cPacket.type = C_PEN;
    (cPacket.color).r=-1;
    (cPacket.color).g=-1;
    (cPacket.color).b=-1;
    (cPacket.color).id=-1;
    cPacket.tool_width = -1;
    (cPacket.mouse).xcor= -1;
    (cPacket.mouse).ycor= -1;
    (cPacket.mouse).lastx= -1;
    (cPacket.mouse).lasty= -1;
}

/*
 * Called iteratively in main.c
 */
void run_client(){
    update_cPacket(); // Applies the local conditions to the cPacket buffer

    if (mouse.xcor >= 0 || mouse.ycor >= 0) {
        b = write( socket_id, &cPacket, sizeof(cPacket));
        if (b == -1) printf("write returned %d with an error \"%s\"\n", b, strerror(errno));
    }

    clear_buffer(); // Sets cPacket to nonsense values

    b = read( socket_id, &cPacket, sizeof(cPacket));
    /*if (b == -1) printf("read returned %d with an error \"%s\"\n", b, strerror(errno));*/
    /*printf("\tReceived %d bytes\n", b);*/

}

void cleanup_client() {
    //unregister with the current document using a DISCONNECT message
    cPacket.type = C_DISCONNECT;
    b = write(socket_id, &cPacket, sizeof(cPacket) +1);
    printf("disconnected from server\n\n\n");
    close(socket_id);
}

