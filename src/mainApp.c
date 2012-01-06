#include <stdio.h>
#include <unistd.h>

//I will be the user interactions process
//First, I will fork off a canvas and client
//Then I'll package  mouse events/movements for the rest of my life

void dummyCanvasFunction(){}
void dummyClientFunction(){}
void dummyInteractionsFunction(){}

int main(){
  int canvas, client;
  int canvasPipe[2], clientPipe[2];
  pipe(canvasPipe);
  pipe(clientPipe);

  canvas = fork();
  client = fork();

  if(canvas == 0)
    dummyCanvasFunction();
  else if(client == 0)
    dummyClientFunction();
  else
    dummyInteractionsFunction();
  return 1;
}
