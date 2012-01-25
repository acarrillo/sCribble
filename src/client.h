#include "defs.h"

void run_client();
void initClient(char*);

int socket_id;
extern cribblePacket cPacket;
int b;                        // Number of bytes written
struct sockaddr_in sock;
