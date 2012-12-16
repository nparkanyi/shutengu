#include <stdio.h>
#include <stdlib.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"

const int SCREEN_WIDTH=640;				//define screen properties
const int SCREEN_HEIGHT=480;
const int SCREEN_BPP=32;

SDL_Surface *screen=NULL;				//set up SDL elements
SDL_Surface *dots=NULL;
SDL_Event event;
SDL_Rect clip[4];						//the four circles in circle.png

bool initSDL();
bool loadContent();
void printb(int x,int y,SDL_Surface *orig,SDL_Surface *target,SDL_Rect *clip);//blit with offsets
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

	//Clip range for the top left
    clip[ 0 ].x = 0;
    clip[ 0 ].y = 0;
    clip[ 0 ].w = 100;
    clip[ 0 ].h = 100;

    //Clip range for the top right
    clip[ 1 ].x = 100;
    clip[ 1 ].y = 0;
    clip[ 1 ].w = 100;
    clip[ 1 ].h = 100;

    //Clip range for the bottom left
    clip[ 2 ].x = 0;
    clip[ 2 ].y = 100;
    clip[ 2 ].w = 100;
    clip[ 2 ].h = 100;

    //Clip range for the bottom right
    clip[ 3 ].x = 100;
    clip[ 3 ].y = 100;
    clip[ 3 ].w = 100;
    clip[ 3 ].h = 100;

	SDL_FillRect( screen, &screen->clip_rect, SDL_MapRGB( screen->format, 0xFF, 0xFF, 0xFF ) );

	printb(0,0,dots,screen,&clip[0]);
	printb(540,0,dots,screen,&clip[1]);
	printb(0,380,dots,screen,&clip[2]);
	printb(540,380,dots,screen,&clip[3]);

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
		printf("Couldn't start screen! %s\n",SDL_GetError());
		return false;
	}

	//set window title
	SDL_WM_SetCaption("Shutengu!!",NULL);

	return true;
}

bool loadContent(){
	//load images
	dots=IMG_Load("circle.png");

	if(dots==NULL){
		printf("Couldn't load the image!\n");
		return false;
	}

	return true;
}

void printb(int x,int y,SDL_Surface *orig,SDL_Surface *target,SDL_Rect *clip=NULL){
	SDL_Rect offset;		//pos offsets must be in a rect

	offset.x=x;
	offset.y=y;

	SDL_BlitSurface(orig,clip,target,&offset);
}

void cleanup(){
	//clean up
	SDL_FreeSurface(dots);
	SDL_Quit();
}
