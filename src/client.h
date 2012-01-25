#include "defs.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>

void run_client();
void initClient(char*);

int socket_id;
int b;                        // Number of bytes written
struct sockaddr_in sock;

extern cribblePacket cPacket;
extern Mouse mouse;
extern Color color;
