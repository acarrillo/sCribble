/*
 * SDL framework borrowed from http://www.parallelrealities.co.uk/
 */
#include "graphics.h"

void drawPixel(int x, int y, int color) {
    Uint32 *pixel;

    pixel = (Uint32*) screen->pixels + x + y; // Magic pointer arithmetic; pixels are stored in linear array
    *pixel = color;
}
void drawFilledRect(int xi, int yi, int len, int width, int r, int g, int b) {
    int color, ytimesw;
    color = SDL_MapRGB(screen->format, r, g, b);
    int x, y;
    for (y=0; y < len; y++) {
        ytimesw = (yi+y)*screen->pitch/4;
        for (x=0; x < width; x++) {
            if (xi+x < screen->w && y+yi < screen->h && xi+x >= 0 && yi+y >= 0) {
                drawPixel(xi+x, ytimesw, color);
            }
        }
    }
}

/*
 * TODO: Make this work better for vertical and diagonal lines.
 */
void drawLine(int xi, int yi, int xf, int yf, int r, int g, int b) {
    int color;
    int x, y, ytimesw;
    color = SDL_MapRGB(screen->format, r, g, b);

    /*SDL_Rect rect;*/

    float slope;
    slope = ((float)yf-yi)/((float)xf-xi);
    /*printf("%sslope:%2.2f\n",gtag, slope);*/
    if(slope >= 0){}
    for ( y = 0; y <= (yf-yi); y++) {
        ytimesw = (y+yi)*screen->pitch/4;
        for( x = 0; x < (xf-xi); x++ ) {
            if (fabs(x*slope - y) < tool_width) {
                /*rect.x = xi+x-(tool_width/2);*/
                /*rect.y = yi+y;*/
                /*rect.w = rect.h = tool_width;*/
                /*SDL_FillRect(screen, &rect, color);*/
                if (xi+x < screen->w && y+yi < screen->h && xi+x >= 0 && yi+y >= 0) {
                    drawPixel(xi+x, ytimesw, color);
                }
            }
        }
    }
}

//SDL_TTF THINGS START HERE!
TTF_Font *loadFont(char *name, int size)
{
	/* Use SDL_TTF to load the font at the specified size */
	
	TTF_Font *font = TTF_OpenFont(name, size);

	if (font == NULL)
	{
		printf("Failed to open Font %s: %s\n", name, TTF_GetError());

		exit(1);
	}
	
	return font;
}

void closeFont(TTF_Font *font)
{
	/* Close the font once we're done with it */
	
	if (font != NULL)
	{
		TTF_CloseFont(font);
	}
}

void drawString(char *text, TTF_Font *font)
{
	SDL_Rect dest;
	SDL_Surface *surface;
	SDL_Color foregroundColor, backgroundColor;
	
	foregroundColor.r = 0;
	foregroundColor.g = 0;
	foregroundColor.b = 0;
	
	backgroundColor.r = 255;
	backgroundColor.g = 255;
	backgroundColor.b = 255;
	
	/* Use SDL_TTF to generate a string image, this returns an SDL_Surface */

	surface = TTF_RenderUTF8_Shaded(font, text, foregroundColor, backgroundColor);

	if (surface == NULL)
	{
		printf("Couldn't create String %s: %s\n", text, SDL_GetError());

		return;
	}
	
	/* Blit the entire surface to the screen */

	dest.x = 10;
	dest.y = 430;
	dest.w = surface->w;
	dest.h = surface->h;

	SDL_BlitSurface(surface, NULL, screen, &dest);
	
	/* Free the generated string image */

	SDL_FreeSurface(surface);
}

void Status(char* message, TTF_Font *font){
  drawFilledRect(10,430,20,500,255,255,255);
  drawString(message, font);
}

void updateScreen() {
  //TTF_Font *font;
  //font = loadFont("font/blackWolf.ttf", 16);


    if(SDL_MUSTLOCK(screen)) {
        if(SDL_LockSurface(screen) < 0) return; //Lock surface for directly accessing pixels
    }

    /* If there has been a change to the drawing, draw it */
    if (mouse.xcor >= 0 && mouse.ycor < 450) {
      if(color.r == 255 && color.g == 255 && color.b == 255){
	//Status("Status: Erasing...", font);
      }
      else
	//Status("Status: Drawing...", font);
        /* TODO: Scale up from simple pen tool */
        drawFilledRect(mouse.xcor-(tool_width/2), mouse.ycor-(tool_width/2), tool_width, tool_width, color.r, color.g, color.b);
        if (mouse.lastx >= 0) { // TODO: make sure this works with bottom of screen
            if (mouse.lastx < mouse.xcor-(tool_width/2)) {
                drawLine(mouse.lastx, mouse.lasty, mouse.xcor-(tool_width/2), mouse.ycor-(tool_width/2), color.r, color.g, color.b);
            }
            else {
                drawLine(mouse.xcor-(tool_width/2), mouse.ycor-(tool_width/2), mouse.lastx, mouse.lasty, color.r, color.g, color.b);
            }
        }
        mouse.lastx = mouse.xcor;
        mouse.lasty = mouse.ycor;
    }
    
    
	//ROYGBIV PALETTE
    if (mouse.ycor > 450 && mouse.ycor < 465){
	if(mouse.xcor <= 25){
	  //Status("Status: Pen Color: Black", font);
	  color.r=0;
	  color.g=0;
	  color.b=0;
	}
	else if(mouse.xcor <= 50){
	  //Status("Status: Pen Color: Red", font);
	  color.r=255;
	  color.g=0;
	  color.b=0;
	}
	else if(mouse.xcor <= 75){
	  //Status("Status: Pen Color: Orange", font);
	  color.r=255;
	  color.g=100;
	  color.b=0;
	}
	else if(mouse.xcor <= 100){
	  //Status("Status: Pen Color: Yellow", font);
	  color.r=255;
	  color.g=255;
	  color.b=0;
	}
	else if(mouse.xcor <= 125){
	  //Status("Status: Pen Color: Green", font);
	  color.r=0;
	  color.g=255;
	  color.b=0;
	}
	else if(mouse.xcor <= 150){
	  //Status("Status: Pen Color: Blue", font);
	  color.r=0;
	  color.g=0;
	  color.b=255;
	}
	else if(mouse.xcor <= 175){
	  //Status("Status: Pen Color: Indigo", font);
	  color.r=75;
	  color.g=0;
	  color.b=130;
	}
	else if(mouse.xcor <= 200){
	  //Status("Status: Pen Color: Violet", font);
	  color.r=148;
	  color.g=0;
	  color.b=211;
	}
	//Changing pen size where 1 <= pen size <= 16.
	else if(mouse.xcor <=300 && mouse.xcor >=275 && tool_width > 1){
	  //Status("Status: Pen size decreased", font);
	  tool_width--;	  
	}
	else if(mouse.xcor <=425 && mouse.xcor >=400 && tool_width < 16){
	  //Status("Status: Pen size increased", font);	  
	  tool_width++;
	}
	else if(mouse.xcor >= 550 && mouse.xcor <= 575){
	  //Status("Status: Eraser ON", font);
	  color.r=255;
	  color.g=255;
	  color.b=255;
	}
    }
    //Creates a pen size indicator.
    drawFilledRect(300,460,5,100,255,255,255);
    drawLine(300,462,400,462,0,0,0);
    drawFilledRect(300 + (tool_width * 5),460,5,10,0,0,0);
    
      

    drawLine(0, 425, 650, 425, 0, 0, 0); //DIVIDER LINE

	//draws ROYGBIV palette
    drawFilledRect(0,455,15,25, 0,0,0);
    drawFilledRect(25,455,15,25, 255,0,0);
    drawFilledRect(50,455,15,25, 255,100,0);
    drawFilledRect(75,455,15,25, 255,255,0);
    drawFilledRect(100,455,15,25, 0,255,0);
    drawFilledRect(125,455,15,25, 0,0,255);
    drawFilledRect(150,455,15,25, 75, 0 ,130);
    drawFilledRect(175,455,15,25, 148, 0 , 211);
    
    
	//draws the erasing rectangle.
    drawFilledRect(549,454,17,27,0,0,0);
    drawFilledRect(550,455,15,25,255,255,255);
   
	//draws the grey rectangles that are buttons that change tool_width
    drawFilledRect(275,455,15,25,100,100,100);
    drawFilledRect(400,455,15,25,100,100,100);
      

    //indicates the current color of the pen.
    drawFilledRect(497,452, 21,21, 0,0,0);
    drawFilledRect(499,454, 17,17,255,255,255);
    drawFilledRect(500,455,15, 15,color.r, color.g, color.b);

    
 
    
    closeFont(font);


    if(SDL_MUSTLOCK(screen)) SDL_UnlockSurface(screen); //Unlocks surface, done writing
    SDL_Flip(screen); //Swap image buffers
}
