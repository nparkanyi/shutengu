#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include <stdio.h>
#include <stdlib.h>

void apply_surface(int x,int y,SDL_Surface *source, SDL_Surface *destination,SDL_Rect *clip);
bool init();
bool load_files();
void cleanup();

//The attributes of the screen
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int SCREEN_BPP = 32;

//The surfaces that will be used
SDL_Surface *up=NULL;
SDL_Surface *down=NULL;
SDL_Surface *left=NULL;
SDL_Surface *right=NULL;
SDL_Surface *screen = NULL;
SDL_Surface *background=NULL;
SDL_Surface *message=NULL;
//event structure
SDL_Event event;
//portion of sprite map to be blitted
SDL_Rect clip[4];

int main(int argc,char *args[]){
	bool quit=false;

	if (init()==false){
		return 1;
	}
	if (load_files()==false){
		return 1;
	}

	while (quit==false){
		if (SDL_PollEvent(&event)){
			//if key press
			if (event.type == SDL_KEYDOWN){
				switch(event.key.keysym.sym){
					case SDLK_UP: message=up; break;
					case SDLK_DOWN: message=down; break;
					case SDLK_LEFT: message=left; break;
					case SDLK_RIGHT: message=right; break;
					default:;
				}
			}
			else if (event.type==SDL_QUIT){
				quit=true;
			}
		}

		//displaying your message
		if (message!=NULL){
			//blit it
			apply_surface(0,0,background, screen,NULL);
			apply_surface((SCREEN_WIDTH-message->w)/2,(SCREEN_HEIGHT-message->h)/2,message, screen,NULL);
			//null to refresh
			message=NULL;
		}
		//Update the screen
			if( SDL_Flip( screen ) == -1 )
			{
				return 1;
			}
	}
    //Quit SDL
    cleanup();
    return 0;
}
void apply_surface(int x,int y,SDL_Surface *source, SDL_Surface *destination, SDL_Rect *clip=NULL){
	//temp rectangle to hold offsets
	SDL_Rect offset;
	//give offsets to rectangle
	offset.x=x;
	offset.y=y;

	//blit the surface
	SDL_BlitSurface( source, clip, destination, &offset );
}
bool init(){
	if (SDL_Init(SDL_INIT_EVERYTHING)==-1){
		return false;
	}
	//set up the screen
    screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE );
    if (screen==NULL){
		return false;
    }
    //set window caption
	SDL_WM_SetCaption("Shutengu",NULL);
	//if all is well
	return true;
}
bool load_files(){
	//load images
	up=IMG_Load("arrowup.png");
	down=IMG_Load("arrowdown.png");
	left=IMG_Load("arrowleft.png");
	right=IMG_Load("arrowright.png");

	return true;
}
void cleanup(){
	SDL_FreeSurface(up);
	SDL_FreeSurface(down);
	SDL_FreeSurface(left);
	SDL_FreeSurface(right);
	SDL_Quit();
}
