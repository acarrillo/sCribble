/**
 * subserver.c
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
#include <stdio.h>
#include <unistd.h>
#include <sys/sem.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include "server.h"

void subserve(int socket_client){
  int b, done;
  cribblePacket buffer;
  struct sembuf sop;
  sop.sem_num = 0;
  sop.sem_flg = 0;
  done = 0;

  printf("subserver init'd\n");

 //mark subserver socket as blocking
  int opts = fcntl(socket_client,F_GETFL);
  fcntl(socket_client, F_SETFL, opts & (~O_NONBLOCK));

  while(!done){
    //read from the client (blocks)
    b = read( socket_client, &buffer, sizeof(buffer) );
    if(buffer.type != C_CONNECT){
      printf("Subserver: got %d message\n", buffer.type);      

      //grab the semaphore
      sop.sem_op = -2;
      semop(semid, &sop, 1);
      
      if(buffer.type == C_DISCONNECT){
	sprintf(messagePot->data, "%d", socket_client);
	done = 1;
      }
      
      memcpy(messagePot, &buffer, sizeof(buffer));
      
      //release the semaphore to the main server
      printf("\tSubserver: releasing the semaphore\n");
      sop.sem_op = -1;
      semop(semid, &sop, 1);
    }
  }
}
