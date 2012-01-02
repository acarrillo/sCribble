/* This is a very basic tutorial. It will open a
** 640 x 480 screen and wait for the user to either close
** the window or press escape
*/

#include "main.h"

extern void init(char *);
extern void cleanup(void);
extern void getInput(void);

int main(int argc, char *argv[])
{
	int go;
	
	/* Start up SDL */
	
	init("Parallel Realities Tutorial 1");
	
	/* Call the cleanup function when the program exits */
	
	atexit(cleanup);
	
	go = 1;
	
	/* Loop indefinitely for messages */
	
	while (go == 1)
	{
		getInput();
		
		/* Sleep briefly to stop sucking up all the CPU time */
		
		SDL_Delay(16);
	}
	
	/* Exit the program */
	
	exit(0);
}
