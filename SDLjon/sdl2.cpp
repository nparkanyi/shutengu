#include <stdio.h>
#include <stdlib.h>
#include "SDL/SDL.h"

const int SCREEN_WIDTH=640;				//define screen properties
const int SCREEN_HEIGHT=480;
const int SCREEN_BPP=32;

SDL_Surface *message=NULL;				//declare surfaces
SDL_Surface *background=NULL;
SDL_Surface *screen=NULL;

SDL_Surface *loadImg(char *filename);	//load image and convert to screen's format
void apply_surface(int x,int y,SDL_Surface *orig,SDL_Surface *target);//blit with offsets

int main(int argc,char *args[]){
	//start SDL, respond if problems occur
	if(SDL_Init(SDL_INIT_EVERYTHING)<0){
		printf("Couldn't start SDL! %s\n",SDL_GetError());
		return 1;
	}

	//set up the screen, respond if problems occur
	screen=SDL_SetVideoMode(SCREEN_WIDTH,SCREEN_HEIGHT,SCREEN_BPP,SDL_SWSURFACE);
	if(screen==NULL){
		printf("Couldn't start screen!\n");
		return 1;
	}

	//set window title
	SDL_WM_SetCaption("Shutengu!!",NULL);

	//load images
	message=loadImg("cb.bmp");
	background=loadImg("bg.bmp");

	//apply bitmaps
	apply_surface(0,0,background,screen);
	apply_surface(180,140,message,screen);

	//update the screen, respond if problems occur
	if(SDL_Flip(screen)<0){
		printf("Couldn't update the screen!\n");
		return 1;
	}

	//hold for 2 seconds
	SDL_Delay(2000);

	//clean up
	SDL_FreeSurface(message);
	SDL_FreeSurface(background);
	SDL_Quit();
	return 0;
}

SDL_Surface *loadImg(char *filename){
	SDL_Surface *loadImg=NULL;				//image input
	SDL_Surface *optImg=NULL;				//image output

	loadImg=SDL_LoadBMP(filename);

	if(loadImg!=NULL){
		optImg=SDL_DisplayFormat(loadImg);	//convert to screen's format
		SDL_FreeSurface(loadImg);			//remove input from memory
	}

	return optImg;
}

void apply_surface(int x,int y,SDL_Surface *orig,SDL_Surface *target){
	SDL_Rect offset;		//pos offsets must be in a rect

	offset.x=x;
	offset.y=y;

	SDL_BlitSurface(orig,NULL,target,&offset);
}
