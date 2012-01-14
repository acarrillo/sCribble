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



void updateScreen() {

    if(SDL_MUSTLOCK(screen)) {
        if(SDL_LockSurface(screen) < 0) return; //Lock surface for directly accessing pixels
    }

    /* If there has been a change to the drawing, draw it */
    if (mouse.xcor >= 0 && mouse.ycor < 450) {
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
    else if (mouse.ycor > 450 && mouse.ycor < 465){
	if(mouse.xcor <= 25){
	  color.r=0;
	  color.g=0;
	  color.b=0;
	}
	else if(mouse.xcor <= 50){
	  color.r=255;
	  color.g=0;
	  color.b=0;
	}
	else if(mouse.xcor <= 75){
	  color.r=255;
	  color.g=100;
	  color.b=0;
	}
	else if(mouse.xcor <= 100){
	  color.r=255;
	  color.g=255;
	  color.b=0;
	}
	else if(mouse.xcor <= 125){
	  color.r=0;
	  color.g=255;
	  color.b=0;
	}
	else if(mouse.xcor <= 150){
	  color.r=0;
	  color.g=0;
	  color.b=255;
	}
	else if(mouse.xcor <= 175){
	  color.r=75;
	  color.g=0;
	  color.b=130;
	}
	else if(mouse.xcor <= 200){
	  color.r=148;
	  color.g=0;
	  color.b=211;
	}
	else{
	  color.r=255;
	  color.g=255;
	  color.b=255;
	}
      }
      else if (mouse.ycor >= 465){
	if(mouse.xcor > 110 && mouse.xcor < 135 && tool_width < 15){
	  tool_width++;
	}
	if(mouse.xcor > 60 && mouse.xcor < 85 && tool_width > 1){
	  tool_width--;
	}
      }
      

    drawLine(0, 450, 650, 450, 0, 0, 0); //DIVIDER LINE

    drawFilledRect(0,450,15,25, 0,0,0);
    drawFilledRect(25,450,15,25, 255,0,0);
    drawFilledRect(50,450,15,25, 255,100,0);
    drawFilledRect(75,450,15,25, 255,255,0);
    drawFilledRect(100,450,15,25, 0,255,0);
    drawFilledRect(125,450,15,25, 0,0,255);
    drawFilledRect(150,450,15,25, 75, 0 ,130);
    drawFilledRect(175,450,15,25, 148, 0 , 211);

    
    


    drawFilledRect(60,465,15,25,100,100,100);
    drawFilledRect(110,465,15,25,100,100,100);
    

    

    //indicates the current color of the pen.
    drawFilledRect(500,450,15, 15,color.r, color.g, color.b);

    drawFilledRect(500,450,15, 15,color.r, color.g, color.b); //DRAW THE CURRENT COLOR

    if(SDL_MUSTLOCK(screen)) SDL_UnlockSurface(screen); //Unlocks surface, done writing
    SDL_Flip(screen); //Swap image buffers
}
