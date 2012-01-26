/**
 * server.h
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
#define AM_SERVER 1
#include "../defs.h"

#define CLIENT_LIST_KEY 27422533
#define MESSAGE_POT_KEY 27422536
#define MESSAGE_POT_SEM 27422537

#define POT_FULL 0
#define POT_ADDING 1
#define POT_EMPTY 3

int semid;
cribblePacket* messagePot;
int clientList[64];
