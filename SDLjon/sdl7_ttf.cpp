#include <stdio.h>
#include <stdlib.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_ttf.h"

const int SCREEN_WIDTH=640;				//define screen properties
const int SCREEN_HEIGHT=480;
const int SCREEN_BPP=32;

SDL_Surface *screen=NULL;				//set up SDL elements
SDL_Surface *msg=NULL;
SDL_Surface *bg=NULL;
SDL_Event event;
TTF_Font *font=NULL;					//the font to load
SDL_Color textColor={255,255,255};		//font color

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
		printf("Loading things didn't work!\n");
		return 1;
	}

	///THINGS GO HERE
	//render text using this font
	msg=TTF_RenderText_Blended(font,"Shutengu!",textColor);
	if(msg==NULL){
		printf("Couldn't render text!\n");
		return 1;
	}

	//content to the screen
	printb(0,0,bg,screen,NULL);
	printb(250,200,msg,screen,NULL);

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

	//start up SDL_ttf, respond if problems
	if(TTF_Init()<0){
		printf("Couldn't start SDL_ttf!\n");
		return false;
	}

	//set window title
	SDL_WM_SetCaption("Shutengu!!",NULL);

	return true;
}

bool loadContent(){
	//load things
	bg=IMG_Load("horizon.png");
	font=TTF_OpenFont("openbold.ttf",32);

	if(bg==NULL){
		printf("Couldn't load the image!\n");
		return false;
	}
	if(font==NULL){
		printf("Couldn't load the font!\n");
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
	SDL_FreeSurface(bg);
	SDL_FreeSurface(msg);
	TTF_CloseFont(font);
	TTF_Quit();
	SDL_Quit();
}
