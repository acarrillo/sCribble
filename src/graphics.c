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
        drawFilledRect(startx, starty, tool_width, tool_width, r, g, b);

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

	SDL_Surface *temp = IMG_Load(name);
	SDL_Surface *image;

	if (temp == NULL)
	{
		printf("Failed to load image %s\n", name);

		return NULL;
	}

	/* Make the background transparent */

	SDL_SetColorKey(temp, (SDL_SRCCOLORKEY|SDL_RLEACCEL), SDL_MapRGB(temp->format, 0, 0, 0));

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
    SDL_Surface *image, *image2, *image3;
    image = loadImage("gfx/color_line.jpg");
    drawImage(image, 225,450);
    image2 = loadImage("gfx/eraser.jpg");
    drawImage(image2, 600,450);
    /*image3 = loadImage("gfx/c_squared2.bmp");*/
    drawImage(image3, 6, 448);
    SDL_FreeSurface(image);
    SDL_FreeSurface(image2);

}


/*--- draws a grey border around drawing screen ---*/
void drawBorder() {
    int k_color;
    k_color = 50;
    drawFilledRect(0, 0, BORDER_WIDTH, screen->w, k_color, k_color, k_color); // Top
    drawFilledRect(0, 0, screen->h, BORDER_WIDTH, k_color, k_color, k_color); // Left
    drawFilledRect(0, screen->h - BORDER_WIDTH, BORDER_WIDTH, screen->w, k_color, k_color, k_color); // Bottom
    drawFilledRect(screen->w - BORDER_WIDTH, 0, screen->h, BORDER_WIDTH, k_color, k_color, k_color); // Bottom
}

void unpressAllColorButton() {

    SDL_Surface *black, *red, *orange, *yellow, *green, *blue, *indigo, *violet, *white;
    black = loadImage("gfx/black_up.bmp");
    red = loadImage("gfx/red_up.bmp");
    orange = loadImage("gfx/orange_up.bmp");
    yellow = loadImage("gfx/yellow_up.bmp");
    green = loadImage("gfx/green_up.bmp");
    blue = loadImage("gfx/blue_up.bmp");
    indigo = loadImage("gfx/indigo_up.bmp");
    violet = loadImage("gfx/violet_up.bmp");
    white = NULL; //TODO: get un-lazy about this

    int i = 0;
    drawImage(black, C_SQUARE*(i++) + BORDER_WIDTH, 448);
    drawImage(red, C_SQUARE*(i++) + BORDER_WIDTH, 449);
    drawImage(orange, C_SQUARE*(i++) + BORDER_WIDTH, 449);
    drawImage(yellow, C_SQUARE*(i++) + BORDER_WIDTH, 449);
    drawImage(green, C_SQUARE*(i++) + BORDER_WIDTH, 449);
    drawImage(blue, C_SQUARE*(i++) + BORDER_WIDTH, 449);
    drawImage(indigo, C_SQUARE*(i++) + BORDER_WIDTH, 449);
    drawImage(violet, C_SQUARE*(i++) + BORDER_WIDTH, 449);

    SDL_FreeSurface(black);
    SDL_FreeSurface(red);
    SDL_FreeSurface(orange);
    SDL_FreeSurface(yellow);
    SDL_FreeSurface(green);
    SDL_FreeSurface(blue);
    SDL_FreeSurface(indigo);
    SDL_FreeSurface(violet);
}
void depressColorButton() {
    SDL_Surface *button;
    /*button = loadImage("gfx/c_squared2_red_down.bmp");*/
    switch(color.id) {
        case BLACK:
            break;
        case RED:
            drawImage(button, 6, 488);
            break;
        case ORANGE:
            break;
        case YELLOW:
            break;
        case BLUE:
            break;
        case INDIGO:
            break;
        case VIOLET:
            break;
        default:
            break;
    }
    SDL_FreeSurface(button);
}

/*
 * Called iteratively
 */
void updateScreen() {
    TTF_Font *font;
    font = loadFont("font/blackWolf.ttf", 16);

    unpressAllColorButton();
    setImages();

    depressColorButton(); // Makes the right color button look filled in

    if(SDL_MUSTLOCK(screen)) {
        if(SDL_LockSurface(screen) < 0) return; //Lock surface for directly accessing pixels
    }

    /* If there has been a change to the drawing, draw it */
    if (mouse.xcor >= 0 && mouse.ycor < 450) {
        if(color.r == 255 && color.g == 255 && color.b == 255){
            Status("Status: Erasing...", font);
        }
        else
            Status("Status: Drawing...", font);
        if(mouse.lastx >= 0){
            line(mouse.lastx, mouse.lasty, mouse.xcor, mouse.ycor, color.r, color.g, color.b);
        }
        mouse.lastx = mouse.xcor;
        mouse.lasty = mouse.ycor;
    }



    //ROYGBIV PALETTE
    else if (mouse.ycor > 450 && mouse.ycor < 450+C_SQUARE){ // If the mouse is in range of the palette area
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
        else if(mouse.xcor <=300 && mouse.xcor >=275 && tool_width > 1){
            Status("Status: Pen size decreased", font);
            tool_width--;
        }
        else if(mouse.xcor <=425 && mouse.xcor >=400 && tool_width < 16){
            Status("Status: Pen size increased", font);
            tool_width++;
        }
        else if(mouse.xcor >= 305 && mouse.xcor <= 390){
            Status("Status: Pen size changing", font);
            tool_width = (mouse.xcor - 300) / 5;
        }
        else if(mouse.xcor >= 550 && mouse.xcor <= 575){
            Status("Status: Eraser ON", font);
            color.r=255;
            color.g=255;
            color.b=255;
        }
    }

    //Creates a pen size indicator.
    drawFilledRect(300,460,5,100,255,255,255);

    drawFilledRect(300 + (tool_width * 5),460,5,10,0,0,0);





    /* If palette square is depressed, make it look so */
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
    //Draw border
    drawBorder();



    closeFont(font);


    if(SDL_MUSTLOCK(screen)) SDL_UnlockSurface(screen); //Unlocks surface, done writing
    SDL_Flip(screen); //Swap image buffers
}
