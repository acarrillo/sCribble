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
    
    //if the mouse is under the divider line, then it will take in a new color.
    //if the mouse is on the black button, it will reset the RGB values.
    else if(mouse.ycor > 450 && mouse.xcor <= 25){
      color.r=0;
      color.g=0;
      color.b=0;
    }
    //change R values by intervals of 50.
    else if(mouse.ycor > 450 && mouse.xcor <= 150){
      color.r = (Uint8)mouse.xcor / 25 * 50;   
    }
    //change G values by intervals of 50.
    else if(mouse.ycor >450 && mouse.xcor <= 275){
      //something went wrong with this so I hardcoded the issue out.	
      if(mouse.xcor > 250 && mouse.xcor <=275){
	color.g=250;
      }
      else{
      color.g=(Uint8)mouse.xcor / 25 * 50;
      }
    }
    //changes B values by intervals of 50.
    else if(mouse.ycor > 450 && mouse.xcor <= 400){
      color.b=(Uint8)mouse.xcor / 25 * 50;
    }
    //sets the pen to "erase" (just painting the page white emulates erasing)
    else if(mouse.ycor > 425){ 
      color.r=255;
      color.g=255;
      color.b=255;
    }

    //divider line separates drawing space and the UI space.
    drawLine(0, 450, 650, 450, 0, 0, 0); 

    //draw color indicators.
    drawFilledRect(0,450,15,25, 0,0,0);
    drawFilledRect(25,450,15,25, 50,0,0);
    drawFilledRect(50,450,15,25, 100,0,0);
    drawFilledRect(75,450,15,25, 150,0,0);
    drawFilledRect(100,450,15,25, 200,0,0);
    drawFilledRect(125,450,15,25, 250,0,0);
    drawFilledRect(150,450,15, 25,0,50,0);
    drawFilledRect(175,450,15, 25,0,100,0);
    drawFilledRect(200,450,15, 25,0,150,0);
    drawFilledRect(225,450,15, 25,0,200,0);
    drawFilledRect(250,450,15, 25,0,250,0);
    drawFilledRect(275,450,15, 25,0,0,50);
    drawFilledRect(300,450,15, 25,0,0,100);
    drawFilledRect(325,450,15, 25,0,0,150);
    drawFilledRect(350,450,15, 25,0,0,200);
    drawFilledRect(375,450,15, 25,0,0,250);
    
    //tried to make it easier. for some reason, it didn't work.
    /*
    int xclr;
    int cclr;
    xclr = 0;
    for(cclr = 0; cclr <= 250; cclr + 10){
      drawFilledRect(xclr, 450, 15, 15, cclr, 0, 0);
      xclr += 15;
    }
    for(cclr = 0; cclr <= 250; cclr + 10){
      drawFilledRect(xclr, 450, 15, 15, 0, cclr, 0);
      xclr += 15;
    }
    for(cclr = 0; cclr <= 250; cclr + 10){
      drawFilledRect(xclr, 450, 15, 15, 0, 0, cclr);
      xclr += 15;
    }
    */

    //indicates the current color of the pen.
    drawFilledRect(500,450,15, 15,color.r, color.g, color.b); 


    if(SDL_MUSTLOCK(screen)) SDL_UnlockSurface(screen); //Unlocks surface, done writing
    SDL_Flip(screen); //Swap image buffers
}
