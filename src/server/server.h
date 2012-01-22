#include "../defs.h"

#define CLIENT_LIST_KEY 727422533
#define MESSAGE_POT_KEY 727422536
#define MESSAGE_POT_SEM 727422537

#define POT_FULL 2
#define POT_ADDING 1
#define POT_EMPTY 0

int semid;
cribblePacket* messagePot;
int* clientList;
