/*
 * SDL framework borrowed from http://www.parallelrealities.co.uk/
 */
#include <math.h>
#include "graphics.h"


extern Circle circle;
extern int toolno;
extern int local_tool_width;
extern cribblePacket cPacket;

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


//dwks from 'http://cboard.cprogramming.com/game-programming/67832-line-drawing-algorithm.html'
//Bresenham's line algorithm
void line(int startx, int starty, int endx, int endy, int r, int g, int b) {
    int t, distance;
    int xerr=0, yerr=0, delta_x, delta_y;
    int incx, incy;

    /* compute the distances in both directions */
    delta_x=endx-startx;
    delta_y=endy-starty;

    int color;
    color = SDL_MapRGB(screen->format, r, g, b);

    /* Compute the direction of the increment,
       an increment of 0 means either a horizontal or vertical
       line.
       */
    if(delta_x>0) incx=1;
    else if(delta_x==0) incx=0;
    else incx=-1;

    if(delta_y>0) incy=1;
    else if(delta_y==0) incy=0;
    else incy=-1;

    /* determine which distance is greater */
    delta_x=abs(delta_x);
    delta_y=abs(delta_y);
    if(delta_x>delta_y) distance=delta_x;
    else distance=delta_y;

    /* draw the line */
    for(t=0; t<=distance+1; t++) {
        drawFilledRect(startx, starty, cPacket.tool_width, cPacket.tool_width, r, g, b);

        xerr+=delta_x;
        yerr+=delta_y;
        if(xerr>distance) {
            xerr-=distance;
            startx+=incx;
        }
        if(yerr>distance) {
            yerr-=distance;
            starty+=incy;
        }
    }
}


/*------------------------ SDL_TTF THINGS START HERE! ------------------------------*/
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
  drawFilledRect(0,427,20,900,255,255,255);
  drawString(message, font);
}

/*---------------------- IMAGE BLITTING THINGS START HERE! -------------------------*/
SDL_Surface *loadImage(char *name)
{
	/* Load the image using SDL Image */

	SDL_Surface *temp = SDL_LoadBMP(name);
	SDL_Surface *image;

	if (temp == NULL)
	{
		printf("Failed to load image %s\n", name);

		return NULL;
	}

	/* Make the background transparent */

	SDL_SetColorKey(temp, (SDL_SRCCOLORKEY|SDL_RLEACCEL), SDL_MapRGB(temp->format, 100, 100, 100));

	/* Convert the image to the screen's native format */

	image = SDL_DisplayFormat(temp);

	SDL_FreeSurface(temp);

	if (image == NULL)
	{
		printf("Failed to convert image %s to native format\n", name);

		return NULL;
	}

	/* Return the processed image */

	return image;
}

void drawImage(SDL_Surface *image, int x, int y)
{
	SDL_Rect dest;

	/* Set the blitting rectangle to the size of the src image */

	dest.x = x;
	dest.y = y;
	dest.w = image->w;
	dest.h = image->h;

	/* Blit the entire image onto the screen at coordinates x and y */

	SDL_BlitSurface(image, NULL, screen, &dest);
}

void setImages(){
    SDL_Surface *image;
    image = loadImage("gfx/color_line.bmp");
    drawImage(image, 225,450);
    SDL_FreeSurface(image);

}


/*--- draws a grey border around drawing screen ---*/
void drawBorder() {
    drawFilledRect(0, 0, BORDER_WIDTH, screen->w, BORDER_COLOR, BORDER_COLOR, BORDER_COLOR); // Top
    drawFilledRect(0, 0, screen->h, BORDER_WIDTH, BORDER_COLOR, BORDER_COLOR, BORDER_COLOR); // Left
    drawFilledRect(0, screen->h - BORDER_WIDTH, BORDER_WIDTH, screen->w, BORDER_COLOR, BORDER_COLOR, BORDER_COLOR); // Bottom
    drawFilledRect(screen->w - BORDER_WIDTH, 0, screen->h, BORDER_WIDTH, BORDER_COLOR, BORDER_COLOR, BORDER_COLOR); // Bottom
}

void drawToolsBackground() {
    int k_bkgrd = 100;
    int y_cor = 446; //Warning: not scalable, FIXME

    drawFilledRect(BORDER_WIDTH, y_cor-2, 2, screen->w - BORDER_WIDTH, BORDER_COLOR, BORDER_COLOR, BORDER_COLOR);
    drawFilledRect(BORDER_WIDTH, y_cor, (screen->h - y_cor) - BORDER_WIDTH, screen->w - BORDER_WIDTH, k_bkgrd, k_bkgrd, k_bkgrd);

}

void setUI(){
    //Creates a pen size indicator.
    drawFilledRect(300,460,5,100,255,255,255);

    drawFilledRect(300 + (local_tool_width * 5),460,5,10,0,0,0);

    //draws ROYGBIV palette
    int i = 1; //DIVIDER (|) multiplier
    int magic = 5; //Magic number, dwai
    drawFilledRect(0+BORDER_WIDTH, 448, 1, C_SQUARE*8, 0, 0, 0);                         // outer black border of palette
    drawFilledRect(0+BORDER_WIDTH,455-BORDER_WIDTH,C_SQUARE,C_SQUARE, 0,0,0);            // Black
    drawFilledRect(0+(C_SQUARE*i++), 449, C_SQUARE, 1, 0, 0, 0);                         // DIVIDER (black)
    drawFilledRect(C_SQUARE+BORDER_WIDTH,455-BORDER_WIDTH,C_SQUARE,C_SQUARE, 255,0,0);   // Red
    drawFilledRect(magic+C_SQUARE*i++, 449, C_SQUARE, 1, 0, 0, 0);                           // DIVIDER (black)
    drawFilledRect(50+BORDER_WIDTH,455-BORDER_WIDTH,C_SQUARE,C_SQUARE, 255,100,0);       // Orange
    drawFilledRect(magic+C_SQUARE*i++, 449, C_SQUARE, 1, 0, 0, 0);                           // DIVIDER (black)
    drawFilledRect(75+BORDER_WIDTH,455-BORDER_WIDTH,C_SQUARE,C_SQUARE, 255,255,0);       // Yellow
    drawFilledRect(magic+C_SQUARE*i++, 449, C_SQUARE, 1, 0, 0, 0);                           // DIVIDER (black)
    drawFilledRect(100+BORDER_WIDTH,455-BORDER_WIDTH,C_SQUARE,C_SQUARE, 0,255,0);        // Green
    drawFilledRect(magic+C_SQUARE*i++, 449, C_SQUARE, 1, 0, 0, 0);                           // DIVIDER (black)
    drawFilledRect(125+BORDER_WIDTH,455-BORDER_WIDTH,C_SQUARE,C_SQUARE, 0,0,255);        // Blue
    drawFilledRect(magic+C_SQUARE*i++, 449, C_SQUARE, 1, 0, 0, 0);                           // DIVIDER (black)
    drawFilledRect(150+BORDER_WIDTH,455-BORDER_WIDTH,C_SQUARE,C_SQUARE, 75, 0 ,130);     // Indigo
    drawFilledRect(magic+C_SQUARE*i++, 449, C_SQUARE, 1, 0, 0, 0);                           // DIVIDER (black)
    drawFilledRect(175+BORDER_WIDTH,455-BORDER_WIDTH,C_SQUARE,C_SQUARE, 148, 0 , 211);   // Violet
    drawFilledRect(magic+C_SQUARE*i++, 449, C_SQUARE, 1, 0, 0, 0);                           // DIVIDER (black)

    //draw "pressed in" shadow for the chosen palette color
    //WARNING: The following four lines employ a multitude of precarious magic numbers!  Beware!  This is unscalable! FIXME
    drawFilledRect(BORDER_WIDTH + color.id*C_SQUARE, 448, 4, C_SQUARE, 0, 0, 0); // Upper border
    drawFilledRect(BORDER_WIDTH + (1+color.id)*C_SQUARE - 4, 448, C_SQUARE, 4, 0, 0, 0); // Right border
    drawFilledRect(BORDER_WIDTH + color.id*C_SQUARE, 448, C_SQUARE, 1, 0, 0, 0); // Left border
    drawFilledRect(BORDER_WIDTH + color.id*C_SQUARE, 448 + C_SQUARE - 1, 2, C_SQUARE, 0, 0, 0); // Lower border

    //draws the erasing rectangle.
    drawFilledRect(549,454,17,27,0,0,0);
    drawFilledRect(550,455,15,25,255,255,255);

    //draws the grey rectangles that are buttons that change local_tool_width
    drawFilledRect(275,455,15,25,100,100,100);
    drawFilledRect(400,455,15,25,100,100,100);


    //indicates the current color of the pen.
    drawFilledRect(497,452, 21,21, 0,0,0);
    drawFilledRect(499,454, 17,17,255,255,255);
    drawFilledRect(500,455,15, 15,color.r, color.g, color.b);

    //Draw border
    drawBorder();
}

void modifyPen(TTF_Font *font){
  if (mouse.ycor > 450 && mouse.ycor < 450+C_SQUARE){ // If the mouse is in range of the palette area
        if(mouse.xcor <= C_SQUARE){
            Status("Status: Pen Color: Black", font);
            color.id=BLACK;
            color.r=0;
            color.g=0;
            color.b=0;
        }
        else if(mouse.xcor <= 2*C_SQUARE){
            Status("Status: Pen Color: Red", font);
            color.id=RED;
            color.r=255;
            color.g=0;
            color.b=0;
        }
        else if(mouse.xcor <= 3*C_SQUARE){
            Status("Status: Pen Color: Orange", font);
            color.id=ORANGE;
            color.r=255;
            color.g=100;
            color.b=0;
        }
        else if(mouse.xcor <= 4*C_SQUARE){
            Status("Status: Pen Color: Yellow", font);
            color.id=YELLOW;
            color.r=255;
            color.g=255;
            color.b=0;
        }
        else if(mouse.xcor <= 5*C_SQUARE){
            Status("Status: Pen Color: Green", font);
            color.id=GREEN;
            color.r=0;
            color.g=255;
            color.b=0;
        }
        else if(mouse.xcor <= 6*C_SQUARE){
            Status("Status: Pen Color: Blue", font);
            color.id=BLUE;
            color.r=0;
            color.g=0;
            color.b=255;
        }
        else if(mouse.xcor <= 7*C_SQUARE){
            Status("Status: Pen Color: Indigo", font);
            color.id=INDIGO;
            color.r=75;
            color.g=0;
            color.b=130;
        }
        else if(mouse.xcor <= 8*C_SQUARE){
            Status("Status: Pen Color: Violet", font);
            color.id=VIOLET;
            color.r=148;
            color.g=0;
            color.b=211;
        }

        //Changing pen size where 1 <= pen size <= 16.
        else if(mouse.xcor <=300 && mouse.xcor >=275 && local_tool_width > 1){
            Status("Status: Pen size decreased", font);
            local_tool_width--;
        }
        else if(mouse.xcor <=425 && mouse.xcor >=400 && local_tool_width < 16){
            Status("Status: Pen size increased", font);
            local_tool_width++;
        }
        else if(mouse.xcor >= 305 && mouse.xcor <= 390){
            Status("Status: Pen size changing", font);
            local_tool_width = (mouse.xcor - 300) / 5;
        }
        else if(mouse.xcor >= 550 && mouse.xcor <= 575){
            Status("Status: Eraser ON", font);
            color.r=255;
            color.g=255;
            color.b=255;
        }

	else if(mouse.xcor >= 225 && mouse.ycor >= 450 && mouse.xcor <= 240 && mouse.ycor <= 465){
	  if(toolno == 1){
	    toolno = 2;
	  }
	  else {toolno = 1;
	  }
	}

	else {
	  toolno = 0;
	}
    }
}

void penLine(TTF_Font *font){
  if (cPacket.mouse.xcor >= 0 && cPacket.mouse.ycor < 450) {
        if(color.r == 255 && color.g == 255 && color.b == 255){
            Status("Status: Erasing...", font);
        }
        else
            Status("Status: Drawing...", font);
        if(cPacket.mouse.lastx >= 0){
            //draws a line based on current information.
            //line(mouse.lastx, mouse.lasty, mouse.xcor, mouse.ycor, color.r, color.g, color.b);
            //draws a line based on cribblePacket's information.
            line((cPacket.mouse).lastx, (cPacket.mouse).lasty, (cPacket.mouse).xcor, (cPacket.mouse).ycor, (cPacket.color).r, (cPacket.color).g, (cPacket.color).b);

        }
        mouse.lastx = mouse.xcor;
        mouse.lasty = mouse.ycor;
    }
}

void rasterCircle(int x0, int y0, int radius){
  int f = 1 - radius;
  int ddF_x = 1;
  int ddF_y = -2 * radius;
  int x = 0;
  int y = radius;

  drawFilledRect(x0, y0 + radius,local_tool_width,local_tool_width,color.r,color.g,color.b);
  drawFilledRect(x0, y0 - radius,local_tool_width,local_tool_width,color.r,color.g,color.b);
  drawFilledRect(x0 + radius, y0,local_tool_width,local_tool_width,color.r,color.g,color.b);
  drawFilledRect(x0 - radius, y0,local_tool_width,local_tool_width,color.r,color.g,color.b);

  while(x < y)
  {
    // ddF_x == 2 * x + 1;
    // ddF_y == -2 * y;
    // f == x*x + y*y - radius*radius + 2*x - y + 1;
    if(f >= 0)
    {
      y--;
      ddF_y += 2;
      f += ddF_y;
    }
    x++;
    ddF_x += 2;
    f += ddF_x;
    drawFilledRect(x0 + x, y0 + y,local_tool_width,local_tool_width,color.r,color.g,color.b);
    drawFilledRect(x0 - x, y0 + y,local_tool_width,local_tool_width,color.r,color.g,color.b);
    drawFilledRect(x0 + x, y0 - y,local_tool_width,local_tool_width,color.r,color.g,color.b);
    drawFilledRect(x0 - x, y0 - y,local_tool_width,local_tool_width,color.r,color.g,color.b);
    drawFilledRect(x0 + y, y0 + x,local_tool_width,local_tool_width,color.r,color.g,color.b);
    drawFilledRect(x0 - y, y0 + x,local_tool_width,local_tool_width,color.r,color.g,color.b);
    drawFilledRect(x0 + y, y0 - x,local_tool_width,local_tool_width,color.r,color.g,color.b);
    drawFilledRect(x0 - y, y0 - x,local_tool_width,local_tool_width,color.r,color.g,color.b);
  }
}

//this just draws a circle with radius 25 as of now. please modify.
void penCircle(TTF_Font *font){
  if (mouse.xcor >= 0 && mouse.ycor < 450){
    if(circle.iter == 0){
      Status("Pick center of circle", font);
      circle.savedx = mouse.xcor;
      circle.savedy = mouse.ycor;
      circle.iter = 1;
    }
    if(circle.iter == 1){
      Status("Pick distance to circle's edge (radius)", font);
      rasterCircle(circle.savedx, circle.savedy, 25);
      circle.iter = 0;
    }
  }
}

void drawRectangle(int x1, int y1, int x2, int y2){
  line(x1,y1,x1,y2,color.r,color.g,color.b);
  line(x1,y1,x2,y1,color.r,color.g,color.b);
  line(x1,y2,x2,y2,color.r,color.g,color.b);
  line(x2,y1,x2,y2,color.r,color.g,color.b);
}

void penRect(TTF_Font *font){
  if(mouse.xcor >= 0 && mouse.ycor < 450){
    if(circle.iter == 0){
      Status("Pick upper left hand corner of rectangle", font);
      circle.savedx = mouse.xcor;
      circle.savedy = mouse.ycor;
      circle.iter = 1;
    }
    if(circle.iter == 1){
      Status("Pick bottom right hand corner of rectangle", font);
      drawRectangle(circle.savedx, circle.savedy, circle.savedx + 100, circle.savedy + 100);
      circle.iter = 0;
    }
  }
}

/*
 * Called iteratively
 */

void updateScreen() {
    TTF_Font *font;
    font = loadFont("font/blackWolf.ttf", 16);


    if(SDL_MUSTLOCK(screen)) {
        if(SDL_LockSurface(screen) < 0) return; //Lock surface for directly accessing pixels
    }


    switch(toolno) {
        case 0:
            Status("Status: Pen Line Drawing", font);
            penLine(font);
            break;
        case 1:
            Status("Status: Rectangle Drawing", font);
            penRect(font);
            break;
        case 2:
            Status("Status: Circle Drawing", font);
            penCircle(font);
            break;
    }

    modifyPen(font);

    setUI();

    closeFont(font);


    if(SDL_MUSTLOCK(screen)) SDL_UnlockSurface(screen); //Unlocks surface, done writing
    SDL_Flip(screen); //Swap image buffers
}
