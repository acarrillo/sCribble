void run_client();
void initClient(char*);

int socket_id;
struct cribblePacket buffer;
int b;                        // Number of bytes written
struct sockaddr_in sock;
