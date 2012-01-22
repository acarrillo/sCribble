#include "../defs.h"

#define CLIENT_LIST_KEY 27422533
#define MESSAGE_POT_KEY 27422536
#define MESSAGE_POT_SEM 27422537

#define POT_FULL 0
#define POT_ADDING 1
#define POT_EMPTY 3

int semid;
cribblePacket* messagePot;
int* clientList;
