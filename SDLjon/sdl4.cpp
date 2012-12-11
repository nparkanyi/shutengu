#include <stdio.h>
#include <stdlib.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"

const int SCREEN_WIDTH=640;				//define screen properties
const int SCREEN_HEIGHT=480;
const int SCREEN_BPP=32;

SDL_Surface *image=NULL;				//declare surfaces
SDL_Surface *screen=NULL;
SDL_Event event;

bool initSDL();
bool loadContent();
SDL_Surface *loadImg(char *filename);	//load image and convert to screen's format
void printb(int x,int y,SDL_Surface *orig,SDL_Surface *target);//blit with offsets
void cleanup();

int main(int argc,char *args[]){
	bool quit=false;

	if(initSDL()==false){
		printf("Setting up SDL and co didn't work!\n");
		return 1;
	}

	if(loadContent()==false){
		printf("Loading images didn't work!\n");
		return 1;
	}

	//apply bitmaps
	printb(0,0,image,screen);

	//update the screen, respond if problems occur
	if(SDL_Flip(screen)<0){
		printf("Couldn't update the screen!\n");
		return 1;
	}

	while(quit==false){						//while the user doesn't want to quit
		while(SDL_PollEvent(&event)){		//while there are events to handle
			if(event.type==SDL_QUIT){		//if the user tries to quit
				quit=true;					//break this loop
			}
		}
	}

	SDL_Delay(2000);

	cleanup();
	return 0;
}

bool initSDL(){
	//start SDL, respond if problems occur
	if(SDL_Init(SDL_INIT_EVERYTHING)<0){
		printf("Couldn't start SDL! %s\n",SDL_GetError());
		return false;
	}

	//set up the screen, respond if problems occur
	screen=SDL_SetVideoMode(SCREEN_WIDTH,SCREEN_HEIGHT,SCREEN_BPP,SDL_SWSURFACE);
	if(screen==NULL){
		printf("Couldn't start screen!\n");
		return false;
	}

	//set window title
	SDL_WM_SetCaption("Shutengu!!",NULL);

	return true;
}

bool loadContent(){
	//load images
	image=loadImg("cb.png");

	if(image==NULL){
		printf("Couldn't load the image!\n");
		return false;
	}

	return true;
}

SDL_Surface *loadImg(char *filename){
	SDL_Surface *loadImg=NULL;					//image input
	SDL_Surface *optImg=NULL;					//image output

	loadImg=IMG_Load(filename);

	if(loadImg!=NULL){
		optImg=SDL_DisplayFormatAlpha(loadImg);	//convert to screen's format
		SDL_FreeSurface(loadImg);				//remove input from memory
	}

	return optImg;
}

void printb(int x,int y,SDL_Surface *orig,SDL_Surface *target){
	SDL_Rect offset;		//pos offsets must be in a rect

	offset.x=x;
	offset.y=y;

	SDL_BlitSurface(orig,NULL,target,&offset);
}

void cleanup(){
	//clean up
	SDL_FreeSurface(image);
	SDL_Quit();
}
